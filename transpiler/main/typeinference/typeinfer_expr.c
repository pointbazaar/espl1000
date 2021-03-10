#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"
#include "ast/util/str_ast.h"

#include "code_gen/gen_c_types.h"
#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"


#include "tables/symtable.h"
#include "tables/lvst.h"
#include "tables/sst.h"
#include "tables/stst.h"

struct Expr2Types {
	struct PrimitiveType* p1;
	struct PrimitiveType* p2;
	struct Op* op;
};

static char* ERR_BTW = 
	"Expected BasicTypeWrapped\n";
	
static char* ERR_ST = 
	"Expected SimpleType\n";
	
static char* ERR_PRIMITIVE = 
	"Expected both Types to be primitive\n";

static char* ERR_COULD_NOT_INFER = 
	"[Expr] Could not infer Type\n";

static struct Type* infer_type_expr_primitive(struct ST* st, struct Expr2Types* e2t);

struct Type* infer_type_expr(struct ST* st, struct Expr* expr){

	if(expr->term2 == NULL)
		{ return infer_type_unopterm(st, expr->term1); }
	
	struct UnOpTerm* t1 = expr->term1;
	struct UnOpTerm* t2 = expr->term2;
	struct Op* op = expr->op;

	struct Type* type1 = infer_type_unopterm(st, t1);
	struct Type* type2 = infer_type_unopterm(st, t2);
	
	struct BasicTypeWrapped* btw1 = type1->m1;
	struct BasicTypeWrapped* btw2 = type2->m1;
	
	if(btw1 == NULL || btw2 == NULL){ print_exit(ERR_BTW); }
	
	struct SimpleType* st1 = btw1->simpleType;
	struct SimpleType* st2 = btw2->simpleType;
	
	if(st1 == NULL || st2 == NULL){ print_exit(ERR_ST);}
	
	bool p1 = st1->primitiveType != NULL;
	bool p2 = st2->primitiveType != NULL;
	
	if(!p1 || !p2){ print_exit(ERR_PRIMITIVE); }
	
	struct Expr2Types e2t = {
		.p1 = st1->primitiveType,
		.p2 = st2->primitiveType,
		.op = op
	};
	
	return infer_type_expr_primitive(st, &e2t);
}

static struct Type* infer_type_expr_primitive(struct ST* st, struct Expr2Types* e2t){
	
	struct PrimitiveType* p1 = e2t->p1;
	struct PrimitiveType* p2 = e2t->p2;
	struct Op* op = e2t->op;
	
	if(op->isRelational)
		{ return typeFromStrPrimitive(st, "bool"); }
	
	if(op->isLogical)
		{ return typeFromStrPrimitive(st, "bool"); }
		
	const bool i1 = p1->isIntType;
	const bool i2 = p2->isIntType;
	
	const bool f1 = p1->isFloatType;
	const bool f2 = p2->isFloatType;
	
	const bool c1 = p1->isCharType;
	const bool c2 = p2->isCharType;
	
	if(op->isArithmetic){
		
		if(i1 && i2)
			{ return typeFromStrPrimitive(st, "int"); }
	
		if(f1 && f2)
			{ return typeFromStrPrimitive(st, "float"); }
			
		if((i1 && f2) || (f1 && i2))
			{ return typeFromStrPrimitive(st, "float"); }
			
		if(c1 && c2)
			{ return typeFromStrPrimitive(st, "char"); }

		if((c1 && i2) || (i1 && c2))
			{ return typeFromStrPrimitive(st, "char"); }
	}
	
	if(op->isBitwise)
		{ return typeFromStrPrimitive(st, "int"); }
	
	
	printf("op=%s\n", op->op);
	print_exit(ERR_COULD_NOT_INFER);
	return NULL;
}
