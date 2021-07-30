#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "cg.h"
#include "stmts/cg_stmts.h"
#include "struct/cg_structdecl.h"
#include "subr/cg_subr.h"

#include "code_gen/util/indent.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

#include "typechecker/typecheck.h"

#include "transpile_lambdas.h"

//Analyzer Includes
#include "analyzer/fn/fn_analyzer.h"
#include "analyzer/dead/dead_analyzer.h"
#include "analyzer/halts/halt_analyzer.h"
#include "analyzer/annotation/annotation_analyzer.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/sst/sst_fill.h"
#include "tables/sst/sst_prefill.h"
#include "tables/sst/sst_print.h"
#include "tables/stst/stst.h"
#include "tables/stst/stst_print.h"
#include "tables/symtable/symtable.h"

//counter for generating labels
unsigned int label_count = 0;

static void transpileAST(struct AST* ast, struct Ctx* ctx, char* h_filename);

static void fill_tables(struct AST* ast, struct Ctx* ctx);

// --- ns_transpile - X ---

static void ns_transpile_struct_fwd_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_struct_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_subr_fwd_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_subrs(struct Namespace* ns, struct Ctx* ctx);

static void backfill_lambdas_into_sst(struct AST* ast, struct ST* st);

// --------------------------------------------------------

static void ns_transpile_passthrough_includes(struct Namespace* ns, struct Ctx* ctx);

bool transpileAndWrite(char* c_filename, char* h_filename, struct AST* ast, struct Flags* flags){

	struct Ctx* ctx = make(Ctx);
	
	ctx->tables = makeST(flags->debug);
	
	ctx->error = false;
	ctx->flags = flags;
	
	ctx->indent_level = 0;
	ctx->file        = NULL;
	ctx->c_file      = NULL;
	ctx->header_file = NULL;
	
	ctx->in_try_block   = false;
	ctx->index_try_stmt = 0;

	ctx->c_file      = fopen(c_filename, "w");
	//full buffering for performance
	setvbuf(ctx->c_file, NULL, _IOFBF, BUFSIZ);
	
	if(flags->emit_headers){
		ctx->header_file = fopen(h_filename, "w");
		//full buffering for performance
		setvbuf(ctx->header_file, NULL, _IOFBF, BUFSIZ);
	}
	
	if(ctx->c_file == NULL || (ctx->header_file == NULL && flags->emit_headers)){
		
		printf("could not open output file: ");
		
		printf("%s\n", (ctx->c_file == NULL)?c_filename:h_filename);
		
		freeST(ctx->tables);
		free(ctx);
		
		return false;
	}
	
	transpileAST(ast, ctx, h_filename);

	fclose(ctx->c_file);
	if(flags->emit_headers){ fclose(ctx->header_file); }
	
	freeST(ctx->tables);
	
	const bool status = !(ctx->error);
	
	free(ctx);
	
	return status;
}

static void fill_tables(struct AST* ast, struct Ctx* ctx){
	
	sst_clear(ctx->tables->sst);
	sst_prefill(ctx->tables, ctx->tables->sst);

	for(int i = 0; i < ast->count_namespaces; i++){
		
		struct Namespace* ns = ast->namespaces[i];
		
		add_gen_struct_subrs_sst(ctx, ns);
		
		sst_fill(ctx->tables, ctx->tables->sst, ns);
		stst_fill(ctx->tables->stst, ns);
	
	}
	
	if(ctx->flags->debug){
		sst_print(ctx->tables->sst);
		stst_print(ctx->tables->stst);
	}
}

static void transpileAST(struct AST* ast, struct Ctx* ctx, char* h_filename){
	
	ctx->file = ctx->c_file; //direct output to c file
	
	{
		fprintf(ctx->c_file, "#include <stdbool.h>\n"); //absolutely needed
		fprintf(ctx->c_file, "#include <math.h>\n");  //absolutely needed
		fprintf(ctx->c_file, "#include <inttypes.h>\n"); //absolutely needed

		//used for try-catch
		fprintf(ctx->c_file, "#include <setjmp.h>\n"); //absolutely needed

		//fprintf(ctx->c_file, "#include <stdlib.h>\n"); //absolutely needed, malloc,free,...
		//fprintf(ctx->c_file, "#include <stdio.h>\n");
		//fprintf(ctx->c_file, "#include <string.h>\n");
		//fprintf(ctx->c_file, "#include <assert.h>\n");
		//fprintf(ctx->c_file, "#include <pthread.h>\n");
	}
	
	if(ctx->flags->emit_headers){
		fprintf(ctx->c_file, "#include \"%s\"\n", h_filename);
	}
	
	fill_tables(ast, ctx);
	
	transpileLambdas(ast, ctx->tables);
	
	//RE-FILL the newly created lambda functions
	backfill_lambdas_into_sst(ast, ctx->tables);
		
	const bool checks = typecheck_ast(ast, ctx->tables);
	
	if(!checks){
		ctx->error = true;
		return;
	}
	
	ctx->flags->has_main_fn = sst_contains(ctx->tables->sst, "main");
	
	analyze_functions(ctx->tables, ast);
	analyze_dead_code(ctx->tables, ast);
	analyze_termination(ctx->tables);
	analyze_annotations(ctx->tables, ast);
	
	if(ctx->flags->debug){
		sst_print(ctx->tables->sst);
		stst_print(ctx->tables->stst);
	}
	
	if(ctx->flags->emit_headers){ 
		ctx->file = ctx->header_file; 
		
		//header guards
		char* sym_name = basename(ast->namespaces[0]->name);
		while(*sym_name == '.'){ sym_name++; }
		fprintf(ctx->file, "#ifndef %s_H\n", sym_name);
		fprintf(ctx->file, "#define %s_H\n\n", sym_name);
	}

	//TODO: make sure that the .h file also receives the correct
	//includes to even make the forward declarations (e.g. <stdbool.h>, ...
	for(int i=0; i < ast->count_namespaces; i++) {
		ns_transpile_passthrough_includes(ast->namespaces[i], ctx);
	}
	
	for(int i=0; i < ast->count_namespaces; i++) { 
		ns_transpile_struct_fwd_decls(ast->namespaces[i], ctx); 
	}
	
	for(int i=0; i < ast->count_namespaces; i++) { 
		gen_struct_subr_signatures(ast->namespaces[i], ctx);
		ns_transpile_subr_fwd_decls(ast->namespaces[i], ctx);
	}
	
	if(ctx->flags->emit_headers){
		//header guards
		fprintf(ctx->file, "\n#endif\n");
	}
	
	for(int i=0; i < ast->count_namespaces; i++)
	{ ns_transpile_struct_decls(ast->namespaces[i], ctx); }
	
	ctx->file = ctx->c_file;
	
	for(int i=0; i < ast->count_namespaces; i++){ 
		
		gen_struct_subrs(ast->namespaces[i], ctx);
		ns_transpile_subrs(ast->namespaces[i], ctx);
	}
}

static void ns_transpile_passthrough_includes(struct Namespace* ns, struct Ctx* ctx) {

	for (int i = 0; i < ns->count_includes; i++) {
		fprintf(ctx->file, "%s\n", ns->includes[i]);
	}
}

static void backfill_lambdas_into_sst(struct AST* ast, struct ST* st){
	
	for(int i=0; i < ast->count_namespaces; i++){
		
		struct Namespace* ns = ast->namespaces[i];
		
		for(int j = 0; j < ns->count_methods; j++){
			
			struct Method* m = ns->methods[j];
			
			//name starts with lambda_
			if(strncmp(m->decl->name, "lambda_", strlen("lambda_")) != 0){
				continue;
			}
			
			struct Type* t = method_to_type(m);
			st_register_inferred_type(st, t);
			
			struct SSTLine* line = makeSSTLine2(m, t, ns->name);
			
			sst_add(st->sst, line);
		}
	}
}

static void ns_transpile_struct_fwd_decls(struct Namespace* ns, struct Ctx* ctx){
	
	for(int i=0;i < ns->count_structs; i++){
		
		struct StructDecl* decl = ns->structs[i];
		
		char* name = decl
					   ->type
			           ->struct_type
					   ->type_name;
		
		fprintf(ctx->file, "struct %s;\n", name);
	}
}

static void ns_transpile_struct_decls(struct Namespace* ns, struct Ctx* ctx){
	
	for(int i=0;i < ns->count_structs; i++){
		transpileStructDecl(ns->structs[i], ctx);
	}
}

static void ns_transpile_subr_fwd_decls(struct Namespace* ns, struct Ctx* ctx){

	for(int i=0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		if(sst_get(ctx->tables->sst, m->decl->name)->dead == DEAD_ISDEAD)
			{continue; }
		
		transpileMethodSignature(m, ctx);
		fprintf(ctx->file, ";\n");
	}
}

static void ns_transpile_subrs(struct Namespace* ns, struct Ctx* ctx){

	for(int i=0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		if(sst_get(ctx->tables->sst, m->decl->name)->dead == DEAD_ISDEAD)
			{continue; }
		
		transpileMethod(m, ctx);
	}
}


void transpileStmtBlock(struct StmtBlock* block, struct Ctx* ctx){
	
	fprintf(ctx->file, "{\n");

	for(int i=0; i < block->count; i++){
		(ctx->indent_level) += 1;
		transpileStmt(block->stmts[i], ctx);
		(ctx->indent_level) -= 1;
	}

	indent(ctx);
	fprintf(ctx->file, "}\n");
}
