#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "c_code_gen.h"
#include "const/c_code_gen_const.h"
#include "stmts/c_code_gen_stmts.h"
#include "types/c_code_gen_types.h"
#include "expr/c_code_gen_expr.h"
#include "struct/c_code_gen_struct.h"
#include "subr/c_code_gen_subr.h"

#include "code_gen/util/code_gen_util.h"
#include "code_gen/types/gen_c_types.h"
#include "code_gen/structs/structs_code_gen.h"

#include "typechecker/typecheck.h"

#include "transpile_lambdas.h"

//Analyzer Includes
#include "analyzer/fn_analyzer.h"
#include "analyzer/dead_analyzer.h"
#include "analyzer/halt_analyzer.h"
#include "analyzer/annotation_analyzer.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/sst/sst_prefill.h"
#include "tables/stst/stst.h"
#include "tables/symtable/symtable.h"

//counter for generating labels
unsigned int label_count = 0;

static void transpileAST(struct AST* ast, struct Ctx* ctx, char* c_filename, char* h_filename);

static void fill_tables(struct AST* ast, struct Ctx* ctx);

// --- ns_transpile - X ---

static void ns_transpile_struct_fwd_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_struct_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_subr_fwd_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_subrs(struct Namespace* ns, struct Ctx* ctx);

static void ns_transpile_fwd(struct Namespace* ns, struct Ctx* ctx);

static void backfill_lambdas_into_sst(struct AST* ast, struct SST* sst);

// --------------------------------------------------------

bool transpileAndWrite(char* c_filename, char* h_filename, struct AST* ast, struct Flags* flags){
	
	if(flags->debug){ printf("[Transpiler] transpileAndWrite\n"); }

	struct Ctx* ctx = make(Ctx);
	
	ctx->tables = makeST(flags->debug);
	
	ctx->error = false;
	ctx->flags = flags;
	
	ctx->indentLevel = 0;
	ctx->file        = NULL;
	ctx->c_file      = NULL;
	ctx->header_file = NULL;

	ctx->c_file      = fopen(c_filename, "w");
	if(flags->emit_headers){
		ctx->header_file = fopen(h_filename, "w");
	}
	
	if(ctx->c_file == NULL || (ctx->header_file == NULL && flags->emit_headers)){
		
		printf("could not open output file: ");
		
		printf("%s\n", (ctx->c_file == NULL)?c_filename:h_filename);
		
		freeST(ctx->tables);
		free(ctx);
		
		return false;
	}
	
	transpileAST(ast, ctx, c_filename, h_filename);

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
		
		sst_fill(ctx->tables->sst, ns);
		stst_fill(ctx->tables->stst, ns);
	
	}
	
	if(ctx->flags->debug){
		sst_print(ctx->tables->sst);
		stst_print(ctx->tables->stst);
	}
}

static void transpileAST(struct AST* ast, struct Ctx* ctx, char* c_filename, char* h_filename){
	
	ctx->file = ctx->c_file; //direct output to c file
	
	if(!(ctx->flags->avr)){
		
		fprintf(ctx->c_file, "#include <stdlib.h>\n");
		fprintf(ctx->c_file, "#include <stdio.h>\n");
		fprintf(ctx->c_file, "#include <stdbool.h>\n");
		fprintf(ctx->c_file, "#include <string.h>\n");
		fprintf(ctx->c_file, "#include <math.h>\n");
		fprintf(ctx->c_file, "#include <inttypes.h>\n");
		fprintf(ctx->c_file, "#include <assert.h>\n");
		fprintf(ctx->c_file, "#include <pthread.h>\n");
	}
	
	if(ctx->flags->emit_headers){
		fprintf(ctx->c_file, "#include \"%s\"\n", h_filename);
	}
	
	fill_tables(ast, ctx);
	
	//transpile the LAMBDA parameters
	transpileLambdas(ast, ctx->tables);
	
	//RE-FILL the newly created lambda functions
	backfill_lambdas_into_sst(ast, ctx->tables->sst);
		
	const bool checks = typecheck_ast(ast, ctx->tables);
	
	if(!checks){
		ctx->error = true;
		return;
	}
	
	ctx->flags->has_main_fn = sst_contains(ctx->tables->sst, "main");
	
	analyze_functions(ctx->tables, ast);
	analyze_dead_code(ctx->tables, ast);
	analyze_termination(ctx->tables, ast);
	analyze_annotations(ctx->tables, ast);
	
	if(ctx->flags->debug){
		sst_print(ctx->tables->sst);
	}
	
	if(ctx->flags->emit_headers){ ctx->file = ctx->header_file; }
	
	for(int i=0; i < ast->count_namespaces; i++)
	{ ns_transpile_fwd(ast->namespaces[i], ctx); }
	
	for(int i=0; i < ast->count_namespaces; i++)
	{ ns_transpile_struct_decls(ast->namespaces[i], ctx); }
	
	ctx->file = ctx->c_file;
	
	for(int i=0; i < ast->count_namespaces; i++){ 
		
		gen_struct_subrs(ast->namespaces[i], ctx);
		ns_transpile_subrs(ast->namespaces[i], ctx);
	}
}

static void backfill_lambdas_into_sst(struct AST* ast, struct SST* sst){
	
	for(int i=0; i < ast->count_namespaces; i++){
		
		struct Namespace* ns = ast->namespaces[i];
		
		for(int j = 0; j < ns->count_methods; j++){
			
			struct Method* m = ns->methods[j];
			
			//name starts with lambda_
			if(strncmp(m->name, "lambda_", strlen("lambda_")) != 0){
				continue;
			}
			
			struct SSTLine* line = makeSSTLine2(m);
			sst_add(sst, line);
		}
	}
}

static void ns_transpile_struct_fwd_decls(struct Namespace* ns, struct Ctx* ctx){
	
	for(int i=0;i < ns->count_structs; i++){
		
		struct StructDecl* decl = ns->structs[i];
		
		char* name = decl
					   ->type
			           ->structType
					   ->typeName;
		
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
		
		if(sst_get(ctx->tables->sst, m->name)->dead == DEAD_ISDEAD)
			{continue; }
		
		transpileMethodSignature(m, ctx);
		fprintf(ctx->file, ";\n");
	}
}

static void ns_transpile_subrs(struct Namespace* ns, struct Ctx* ctx){

	for(int i=0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		if(sst_get(ctx->tables->sst, m->name)->dead == DEAD_ISDEAD)
			{continue; }
		
		transpileMethod(m, ctx);
	}
}

static void ns_transpile_fwd(struct Namespace* ns, struct Ctx* ctx){

	ns_transpile_struct_fwd_decls(ns, ctx);
	
	gen_struct_subr_signatures(ns, ctx);
	
	ns_transpile_subr_fwd_decls(ns, ctx);
}

void transpileStmtBlock(struct StmtBlock* block, struct Ctx* ctx){
	
	fprintf(ctx->file, "{\n");

	for(int i=0; i < block->count; i++){
		(ctx->indentLevel) += 1;
		transpileStmt(block->stmts[i], ctx);
		(ctx->indentLevel) -= 1;
	}

	indent(ctx);
	fprintf(ctx->file, "}\n");
}
