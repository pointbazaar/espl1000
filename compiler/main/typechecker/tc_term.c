
//AST Includes
#include "ast/ast.h"

//Table Includes
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "tcctx.h"

static bool tc_constvalue(struct ConstValue* cv) {
	//const values need no typechecking
	switch (cv->kind) {
		case 1: //boolconst
		case 2: //intconst
		case 3: //charconst
		case 5: //hexconst
		case 6: //binconst
			break;
	}
	return true;
}

bool tc_term(struct Term* term, struct TCCtx* tcctx) {

	switch (term->kind) {

		case 4: return tc_methodcall(term->ptr.m4, tcctx);
		case 5: return tc_expr(term->ptr.m5, tcctx);
		case 6: return tc_var(term->ptr.m6, tcctx);

		case 8: return true; //stringconst

		case 11: return true; //lambdas are already handled at this point
		case 12: return tc_constvalue(term->ptr.m12);

		case 13: return tc_mdirect(term->ptr.m13, tcctx);
	}
	return true;
}
