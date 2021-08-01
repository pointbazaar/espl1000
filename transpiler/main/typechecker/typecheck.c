#include <string.h>

//AST Includes
#include "ast/ast.h"

//Table Includes
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "_tc.h"
#include "typecheck.h"
#include "typechecker/util/tc_utils.h"
#include "tcctx.h"

static void init_tcctx(struct TCCtx* tcctx);

bool typecheck_ast(struct AST* ast, struct ST* st){

	struct TCCtx tcctx;

    init_tcctx(&tcctx);
    tcctx.st = st;
	
	for(uint16_t i = 0; i < ast->count_namespaces; i++)
		{ tc_namespace(ast->namespaces[i], &tcctx); }

    if(tcctx.tc_err_count > 0){
		printf("[Typecheck] %d Errors\n", tcctx.tc_err_count);
	}
	
	return tcctx.tc_err_count == 0;
}

void tc_namespace(struct Namespace* n, struct TCCtx* tcctx){
	
	tcctx->current_filename = n->src_path;
	
	for(uint16_t i = 0; i < n->count_methods; i++)
		{ tc_method(n->methods[i], tcctx); }
}

void tc_stmtblock(struct StmtBlock* s, struct TCCtx* tcctx){
	
	tcctx->current_line_num = s->super.line_num;

	for(uint16_t i = 0; i < s->count; i++)
		{ tc_stmt(s->stmts[i], tcctx); }
}

static void init_tcctx(struct TCCtx* tcctx){

    tcctx->tc_err_count = 0;
    tcctx->current_fn   = NULL;

    tcctx->depth_inside_try_stmt = 0;
    tcctx->depth_inside_loop     = 0;
}