#include <string.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"

//Table Includes
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "typecheck_errors.h"
#include "typecheck_utils.h"
#include "typecheck_term.h"
#include "typecheck_expr.h"
#include "typecheck_methodcall.h"
#include "typecheck.h"
#include "tcctx.h"

void tc_term(struct Term* term, struct TCCtx* tcctx){
		
	switch(term->kind){
		
		case  1: break; //boolconst
		case  2: break; //intconst
		case  3: break; //charconst
		case  4: tc_methodcall(term->ptr.m4, tcctx); break;
		case  5: tc_expr(term->ptr.m5, tcctx); break;
		case  6: break; //Variable TODO: typecheck
		case  7: break; //floatconst
		case  8: break; //stringconst
		case  9: break; //hexconst
		case 10: break; //binconst
		case 11: break; //lambdas are already handled at this point
		default:
			error(tcctx, "unhandled");
			break;
	}
}
