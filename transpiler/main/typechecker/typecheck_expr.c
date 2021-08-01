#include <string.h>

//AST Includes
#include "ast/ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "typecheck_unopterm.h"
#include "typecheck_expr.h"
#include "tcctx.h"

void tc_expr(struct Expr* expr, struct TCCtx* tcctx){

	tcctx->current_line_num = expr->super.line_num;

	tc_unopterm(expr->term1, tcctx);
	
	if(expr->op != NULL){
		
		//check if the types match
		//TODO
	
		//no need to typecheck the operator
		tc_unopterm(expr->term2, tcctx);
	}
}
