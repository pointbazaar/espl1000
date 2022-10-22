#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

struct TCError* typecheck_ast(struct AST* ast, struct ST* st, bool print_errors){

	struct TCCtx* tcctx = malloc(sizeof(struct TCCtx));

    init_tcctx(tcctx);
    tcctx->print_errors = print_errors;
    tcctx->st = st;
	
	for(uint16_t i = 0; i < ast->count_namespaces; i++){
	    tc_namespace(ast->namespaces[i], tcctx);
	}

    if(tcctx->tc_err_count > 0 && print_errors){
		printf("[Typecheck] %d Errors\n", tcctx->tc_err_count);
	}

    struct TCError* err = tcctx->tc_first_err;

	free(tcctx);
	
	return err;
}

bool tc_namespace(struct Namespace* n, struct TCCtx* tcctx){
	
	tcctx->current_filename = n->src_path;

	bool has_err = false;
	for(uint16_t i = 0; i < n->count_methods; i++)
		{ has_err |= tc_method(n->methods[i], tcctx); }

    return has_err;
}

bool tc_stmtblock(struct StmtBlock* s, struct TCCtx* tcctx){
	
	tcctx->current_line_num = s->super.line_num;

	bool has_err = false;
	for(uint16_t i = 0; i < s->count; i++)
		{ has_err |= tc_stmt(s->stmts[i], tcctx); }
    return has_err;
}

static void init_tcctx(struct TCCtx* tcctx){

    tcctx->tc_first_err = NULL;
    tcctx->tc_last_err  = NULL;
    tcctx->print_errors = true;
    tcctx->tc_err_count = 0;
    tcctx->current_fn   = NULL;

    tcctx->depth_inside_loop     = 0;
}
