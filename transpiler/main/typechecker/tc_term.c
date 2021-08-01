
//AST Includes
#include "ast/ast.h"

//Table Includes
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "tcctx.h"

static void tc_constvalue(struct ConstValue* cv){
	//const values need no typechecking
	switch (cv->kind) {
		case 1: //boolconst
		case 2: //intconst
		case 3: //charconst
		case 4: //floatconst
		case 5: //hexconst
		case 6: //binconst
			break;
	}
}

void tc_term(struct Term* term, struct TCCtx* tcctx){
		
	switch(term->kind){

		case  4: tc_methodcall(term->ptr.m4, tcctx); break;
		case  5: tc_expr(term->ptr.m5, tcctx); break;
		case  6: tc_var(term->ptr.m6, tcctx); break;

		case  8: break; //stringconst

		case 11: break; //lambdas are already handled at this point
		case 12: tc_constvalue(term->ptr.m12); break;
		default:
			error(tcctx, "[Typechecker] unhandled");
			break;
	}
}
