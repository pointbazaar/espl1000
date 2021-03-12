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

#include "analyzer/fn_analyzer.h"

#include "tables/sst/sst.h"
#include "tables/sst/sst_prefill.h"
#include "tables/stst/stst.h"
#include "tables/symtable/symtable.h"

//counter for generating labels
unsigned int label_count = 0;

static void transpileAST(struct AST* ast, struct Ctx* ctx);

static void fill_tables(struct AST* ast, struct Ctx* ctx);

// --- ns - X ---

static void ns_transpile_struct_fwd_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_struct_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_subr_fwd_decls(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_subrs(struct Namespace* ns, struct Ctx* ctx);

static void ns_transpile_fwd(struct Namespace* ns, struct Ctx* ctx);
static void ns_transpile_rest(struct Namespace* ns, struct Ctx* ctx);
// --------------------------------------------------------

bool transpileAndWrite(char* filename, struct AST* ast, struct Flags* flags){
	
	if(flags->debug){ printf("transpileAndWrite(...)\n"); }

	struct Ctx* ctx = make(Ctx);
	
	ctx->tables = makeST(flags->debug);
	
	ctx->flags = flags;
	
	ctx->indentLevel = 0;

	ctx->file = fopen(filename, "w");
	
	if(ctx->file == NULL){
		printf("could not open output file: %s\n", filename);
		
		freeST(ctx->tables);
		free(ctx);
		
		return false;
	}
	
	transpileAST(ast, ctx);

	fclose(ctx->file);
	
	freeST(ctx->tables);
	free(ctx);
	
	return true;
}

static void fill_tables(struct AST* ast, struct Ctx* ctx){
	
	sst_clear(ctx->tables->sst);
	sst_prefill(ctx->tables, ctx->tables->sst);
	
	for(int i = 0; i < ast->count_namespaces; i++){
		
		struct Namespace* ns = ast->namespaces[i];
		
		sst_fill(ctx->tables->sst, ns, ctx->flags->debug);
		stst_fill(ctx->tables->stst, ns, ctx->flags->debug);
	
	}
}

static void transpileAST(struct AST* ast, struct Ctx* ctx){
	
	if(!(ctx->flags->avr)){
		//in microcontrollers, we cannot assume there will
		//be stdlib 
		
		fprintf(ctx->file, "#include <stdlib.h>\n");
		fprintf(ctx->file, "#include <stdio.h>\n");
		fprintf(ctx->file, "#include <stdbool.h>\n");
		fprintf(ctx->file, "#include <string.h>\n");
		fprintf(ctx->file, "#include <math.h>\n");
		fprintf(ctx->file, "#include <inttypes.h>\n");
		fprintf(ctx->file, "#include <assert.h>\n");
	}
	
	fill_tables(ast, ctx);
	
	ctx->flags->has_main_fn = sst_contains(ctx->tables->sst, "main");
	
	//TODO: re-enable
	//analyze_functions(ctx->tables, ast);
	
	//TODO: instead of transpiling namespaces
	//separately, the forward declarations of all
	//namespaces should be transpiled
	//before any functions/structures
	//are defined
	
	for(int i=0; i < ast->count_namespaces; i++)
	{ ns_transpile_fwd(ast->namespaces[i], ctx); }
	
	for(int i=0; i < ast->count_namespaces; i++)
	{ ns_transpile_struct_decls(ast->namespaces[i], ctx); }
	
	for(int i=0; i < ast->count_namespaces; i++)
	{ ns_transpile_rest(ast->namespaces[i], ctx); }
}

static void ns_transpile_struct_fwd_decls(struct Namespace* ns, struct Ctx* ctx){
	
	//write struct fwd. declarations
	for(int i=0;i < ns->count_structs; i++){
		
		char* name = ns->structs[i]
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
		transpileMethodSignature(ns->methods[i], ctx);
		fprintf(ctx->file, ";\n");
	}
}

static void ns_transpile_subrs(struct Namespace* ns, struct Ctx* ctx){

	for(int i=0; i < ns->count_methods; i++){
		transpileMethod(ns->methods[i], ctx);
	}
}

static void ns_transpile_fwd(struct Namespace* ns, struct Ctx* ctx){

	ns_transpile_struct_fwd_decls(ns, ctx);
	
	gen_struct_subr_signatures(ns, ctx);
	
	ns_transpile_subr_fwd_decls(ns, ctx);
	
	//TODO: transpile the fwd declarations
	//of the generated subroutines for the structures
}

static void ns_transpile_rest(struct Namespace* ns, struct Ctx* ctx){

	//generate the struct specific
	//constructors, destructors,
	//copy-constructors
	//and update the symbol table
	gen_struct_subrs(ns, ctx);
	
	ns_transpile_subrs(ns, ctx);
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