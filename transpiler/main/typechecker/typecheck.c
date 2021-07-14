#include <string.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Analyzer Includes
#include "transpiler/main/analyzer/lv/lv_analyzer.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "typecheck.h"
#include "typecheck_stmts.h"
#include "typecheck_utils.h"
#include "typecheck_errors.h"
#include "tcctx.h"

bool typecheck_ast(struct AST* ast, struct ST* st){

	struct TCCtx tcctx;
	
	tcctx.tc_err_count       = 0;
	tcctx.st               = st;
	tcctx.current_fn        = NULL;
	
	tcctx.depth_inside_try_stmt = 0;
	tcctx.depth_inside_loop     = 0;
	
	for(uint16_t i = 0; i < ast->count_namespaces; i++)
		{ tc_namespace(ast->namespaces[i], &tcctx); }
	
	const uint32_t errCount = tcctx.tc_err_count;
	
	if(errCount > 0){
		printf("[Typecheck] %d Errors\n", errCount);
	}
	
	return errCount == 0;
}

void tc_namespace(struct Namespace* n, struct TCCtx* tcctx){
	
	tcctx->current_filename = n->src_path;
	
	for(uint16_t i = 0; i < n->count_methods; i++)
		{ tc_method(n->methods[i], tcctx); }
}

void tc_method(struct Method* m, struct TCCtx* tcctx){
	
	tcctx->current_line_num = m->super.line_num;

	tcctx->current_fn = m;
	
	lvst_clear(tcctx->st->lvst);
	lvst_fill(m, tcctx->st);
	
	tc_stmtblock(m->block, tcctx);
}

void tc_stmtblock(struct StmtBlock* s, struct TCCtx* tcctx){
	
	tcctx->current_line_num = s->super.line_num;

	for(uint16_t i = 0; i < s->count; i++)
		{ tc_stmt(s->stmts[i], tcctx); }
}

void tc_range(struct Range* r, struct TCCtx* tcctx){
	
	tcctx->current_line_num = r->super.line_num;

	struct Type* t1 = infer_type_expr(tcctx->current_filename, tcctx->st, r->start);
	struct Type* t2 = infer_type_expr(tcctx->current_filename, tcctx->st, r->end);

	if(!is_integer_type(t1) || !is_integer_type(t2)){
		
		char* sRange = str_range(r);
		
		char msg[200];
		sprintf(msg, "\t%s\n", sRange);
		strcat(msg, ERR_RANGE_REQUIRES_INT);
		
		free(sRange);
		
		error(tcctx, msg);
	}
}
