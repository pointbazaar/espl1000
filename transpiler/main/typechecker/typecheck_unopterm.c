//AST Includes
#include "ast/ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Typechecker Includes
#include "typecheck_term.h"
#include "tcctx.h"

void tc_unopterm(struct UnOpTerm* uot, struct TCCtx* tcctx){

	//TODO: check if the optional unary operator is valid
	//for the term

	tc_term(uot->term, tcctx);
}
