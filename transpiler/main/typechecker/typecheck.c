#include <stdio.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Analyzer Includes
#include "transpiler/main/analyzer/lv_analyzer.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "typecheck.h"
#include "typecheck_stmts.h"
#include "typecheck_utils.h"
#include "typecheck_errors.h"
#include "tcctx.h"

//-----------------------------------------------------

void typecheck_ast(struct AST* ast, struct ST* st){

	struct TCCtx tcctx;
	
	tcctx.tcErrCount = 0;
	tcctx.st         = st;
	tcctx.currentFn  = NULL;
	
	for(uint16_t i = 0; i < ast->count_namespaces; i++)
		{ tc_namespace(ast->namespaces[i], &tcctx); }
	
	const uint32_t errCount = tcctx.tcErrCount;
	
	if(errCount > 0){
		
		printf("[Typecheck] %d Errors\n", errCount);
		exit(1);
	}
}

void tc_namespace(struct Namespace* n, struct TCCtx* tcctx){
	
	for(uint16_t i = 0; i < n->count_methods; i++)
		{ tc_method(n->methods[i], tcctx); }
}

void tc_method(struct Method* m, struct TCCtx* tcctx){

	tcctx->currentFn = m;
	
	lvst_fill(m, tcctx->st, false);
	tc_stmtblock(m->block, tcctx);
}

void tc_stmtblock(struct StmtBlock* s, struct TCCtx* tcctx){

	for(uint16_t i = 0; i < s->count; i++)
		{ tc_stmt(s->stmts[i], tcctx); }
}

void tc_range(struct Range* r, struct TCCtx* tcctx){

	struct Type* t1 = infer_type_expr(tcctx->st, r->start);
	struct Type* t2 = infer_type_expr(tcctx->st, r->end);

	if(!is_integer_type(t1) || !is_integer_type(t2)){
		
		char* sRange = strRange(r);
		
		printf("\t%s\n", sRange);
		free(sRange);
		
		error(tcctx, ERR_RANGE_REQUIRES_INT);
	}
}
