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
	struct SimpleType* st1;
	struct SimpleType* st2;
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
	
	bool p1 = st1->isPrimitive;
	bool p2 = st2->isPrimitive;
	
	if(!p1 || !p2){ print_exit(ERR_PRIMITIVE); }
	
	struct Expr2Types e2t = {
		.st1 = st1,
		.st2 = st2,
		.op = op
	};
	
	return infer_type_expr_primitive(st, &e2t);
}

static struct Type* infer_type_expr_primitive(struct ST* st, struct Expr2Types* e2t){
	
	struct SimpleType* st1 = e2t->st1;
	struct SimpleType* st2 = e2t->st2;
	struct Op* op = e2t->op;
	
	if(op->isRelational)
		{ return typeFromStr(st, "bool", true, false); }
	
	if(op->isLogical)
		{ return typeFromStr(st, "bool", true, false); }
		
	const bool i1 = st1->isIntType;
	const bool i2 = st2->isIntType;
	
	const bool f1 = st1->isFloatType;
	const bool f2 = st2->isFloatType;
	
	const bool c1 = st1->isCharType;
	const bool c2 = st2->isCharType;
	
	if(op->isArithmetic){
		
		if(i1 && i2)
			{ return typeFromStr(st, "int", true, true); }
	
		if(f1 && f2)
			{ return typeFromStr(st, "float", true, false); }
			
		if((i1 && f2) || (f1 && i2))
			{ return typeFromStr(st, "float", true, false); }
			
		if(c1 && c2)
			{ return typeFromStr(st, "char", true, false); }

		if((c1 && i2) || (i1 && c2))
			{ return typeFromStr(st, "char", true, false); }
	}
	
	if(op->isBitwise)
		{ return typeFromStr(st, "int", true, true); }
	
	
	printf("Types: \n");
	printf("%s, %s\n", st1->typeName, st2->typeName);
	
	printf("isIntType(1) = %d, isFloatType(2) = %d\n",
		i1?1:0, f2?1:0
	);
	printf("op=%s\n", op->op);
	printf("op-isArithmetic=%d\n", op->isArithmetic?1:0);
	print_exit(ERR_COULD_NOT_INFER);
	return NULL;
}
