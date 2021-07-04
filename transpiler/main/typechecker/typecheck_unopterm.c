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
//#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "typecheck_errors.h"
#include "typecheck_utils.h"
#include "typecheck_op.h"
#include "typecheck_term.h"
#include "typecheck.h"
#include "tcctx.h"

void tc_unopterm(struct UnOpTerm* uot, struct TCCtx* tcctx){

	//TODO: check if the operator is valid
	//for the term?
	
	tc_op(uot->op, tcctx);
	tc_term(uot->term, tcctx);
}
