
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

		case TERM_KIND_CALL: return tc_methodcall(term->ptr.call_term, tcctx);
		case TERM_KIND_EXPR: return tc_expr(term->ptr.expr_term, tcctx);
		case TERM_KIND_VAR: return tc_var(term->ptr.var_term, tcctx);

		case TERM_KIND_STRINGCONST: return true;

		case TERM_KIND_CONSTVALUE: return tc_constvalue(term->ptr.constvalue_term);
		case TERM_KIND_ENUM_VALUE: return true;

		default:
			fprintf(stderr, "%s:%s: unhandled case: %d\n", __FILE__, __func__, term->kind);
			return false;
	}
	return true;
}
