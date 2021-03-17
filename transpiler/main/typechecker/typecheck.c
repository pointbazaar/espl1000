#include <stdio.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"

//Table Includes
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

//Analyzer Includes
#include "transpiler/main/analyzer/lv_analyzer.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "typecheck.h"
#include "typecheck_stmts.h"
#include "typecheck_utils.h"
#include "typecheck_errors.h"

struct ST*     myst      = NULL;
struct Method* currentFn = NULL;

//-----------------------------------------------------

void typecheck_ast(struct AST* ast, struct ST* st){

	myst = st;
	
	for(uint16_t i = 0; i < ast->count_namespaces; i++)
		{ tc_namespace(ast->namespaces[i]); }
}

void tc_namespace(struct Namespace* n){
	
	for(uint16_t i = 0; i < n->count_methods; i++)
		{ tc_method(n->methods[i]); }
}

void tc_method(struct Method* m){

	currentFn = m;
	lvst_fill(m, myst, false);
	tc_stmtblock(m->block);
}

void tc_stmtblock(struct StmtBlock* s){

	for(uint16_t i = 0; i < s->count; i++)
		{ tc_stmt(s->stmts[i]); }
}

void tc_range(struct Range* r){

	struct Type* t1 = infer_type_expr(myst, r->start);
	struct Type* t2 = infer_type_expr(myst, r->end);

	if(!is_integer_type(t1) || !is_integer_type(t2)){
		error(ERR_RANGE_REQUIRES_INT);
	}
}
