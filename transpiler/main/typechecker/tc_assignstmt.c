#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "ast/util/equals_ast.h"

//Table Includes
#include "tables/symtable/symtable.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "typecheck.h"
#include "tcctx.h"

static bool check_type_rules_assign(struct AssignStmt* a, struct TCCtx* tcctx);

bool tc_assignstmt(struct AssignStmt* a, struct TCCtx* tcctx){

	tcctx->current_line_num = a->super.line_num;
	
	struct LVSTLine* line = lvst_get(tcctx->st->lvst, a->var->simple_var->name);
	
	if(line->read_only){
		error(tcctx, "variable can only be read but not written to.", TC_ERR_VAR_READONLY);
	}

	if(is_malloc(a->expr)){ 
		return tc_expr(a->expr, tcctx);
	}

    if(!tc_var(a->var, tcctx)){return false;}

    if(!tc_expr(a->expr, tcctx)){return false;}

    return check_type_rules_assign(a, tcctx);
}

static bool check_type_rules_assign(struct AssignStmt* a, struct TCCtx* tcctx){

    struct Type* right = infer_type_expr(tcctx->current_filename, tcctx->st, a->expr);

    struct Type* left = a->opt_type;

    if(a->opt_type == NULL){
        left = infer_type_variable(tcctx->current_filename, tcctx->st, a->var);
    }
	
	if(is_integer_type(left) && is_integer_type(right)){ return true; }
	
	if(!eq_type(left, right)){
	
		char* str_t1 = str_type(left);
		char* str_t2 = str_type(right);
		
		char* str_a  = str_assign_stmt(a);
		
		char msg[200];
		
		sprintf(msg, "\t%s\nexpected type: %s, actual type: %s\n", str_a, str_t1, str_t2);
		strcat(msg, ERR_ASSIGN_TYPES_MISMATCH);
		
		free(str_t1);
		free(str_t2);
		free(str_a);
	
		error(tcctx, msg, TC_ERR_ASSIGN_TYPE_MISMATCH);
        return false;
	}
    return true;
}
