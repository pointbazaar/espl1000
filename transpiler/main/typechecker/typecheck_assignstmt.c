#include <string.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "ast/util/equals_ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "typecheck_errors.h"
#include "typecheck_utils.h"
#include "typecheck_stmts.h"
#include "typecheck_methodcall.h"
#include "typecheck.h"
#include "tcctx.h"

void tc_assignstmt(struct AssignStmt* a, struct TCCtx* tcctx){

	tcctx->current_line_num = a->super.line_num;

	//we make an exception
	//TODO: only make exception for array types
	//and other applicable types
	if(is_malloc(a->expr))
		{ return; }

	struct Type* rightType = 
		infer_type_expr(tcctx->current_filename, tcctx->st, a->expr);
	
	struct Type* leftType = a->optType;
	
	if(a->optType == NULL){
		leftType  = infer_type_variable(tcctx->current_filename, tcctx->st, a->var);
	}
	
	if(is_integer_type(leftType) 
	&& is_integer_type(rightType))
		{ return; }
	
	if(!eq_type(leftType, rightType)){
	
		char* str_t1 = strType(leftType);
		char* str_t2 = strType(rightType);
		
		char* str_a  = strAssignStmt(a);
		
		char msg[200];
		
		sprintf(msg, "\t%s\nexpected type: %s, actual type: %s\n", str_a, str_t1, str_t2);
		strcat(msg, ERR_ASSIGN_TYPES_MISMATCH);
		
		free(str_t1);
		free(str_t2);
		free(str_a);
	
		error(tcctx, msg);
	}
}
