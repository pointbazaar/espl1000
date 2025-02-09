#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <typechecker/type_contains/tc_type_contains.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "ast/util/equals_ast.h"

//Table Includes
#include "tables/symtable/symtable.h"

//Typeinference Includes
#include "compiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "typecheck.h"
#include "tcctx.h"

bool tc_local_var_decl_stmt(struct LocalVarDeclStmt* a, struct TCCtx* tcctx) {

	if (tcctx->debug) {
		printf("[debug] typecheck local var decl stmt\n");
	}

	tcctx->current_line_num = a->super.line_num;

	return true;
}
