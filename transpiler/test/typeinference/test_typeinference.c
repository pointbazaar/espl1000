#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "ast/ast.h"
#include "transpiler/main/tables/symtable.h"

#include "transpiler/main/tables/lvst.h"
#include "transpiler/main/tables/sst.h"
#include "transpiler/main/tables/stst.h"
#include "transpiler/main/tables/symtable.h"

#include "transpiler/main/typeinference/typeinference.h"

#include "test_typeinference.h"

void test_infer_type_expr(bool debug){

	struct ST* st = makeST(debug);
	
	struct Expr* expr = malloc(sizeof(struct Expr));
	struct Term* term = malloc(sizeof(struct Term));
	struct IntConst* ic = malloc(sizeof(struct IntConst));
	struct UnOpTerm* uopt = malloc(sizeof(struct UnOpTerm));
	
	ic->value = 3;
	
	term->kind = 2;
	term->ptr.m2 = ic;
	
	uopt->op = NULL;
	uopt->term = term;
	
	expr->term1 = uopt;
	expr->op    = NULL;
	expr->term2 = NULL;

	struct Type* t = inferTypeExpr(st, expr, debug);

	assert(t != NULL);
	
	assert(t->m1 != NULL);
	assert(t->m1->simpleType != NULL);
	
	assert(t->m1->simpleType->isPrimitive);
	assert(t->m1->simpleType->isIntType);
	
	assert(t->m1->simpleType->typeParamCount == 0);
	
	assert(strcmp(t->m1->simpleType->typeName, "int") == 0);
}
