#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ast/util/str_ast.h>

#include "ast/ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#include "tables/symtable/symtable.h"

struct Expr2Types {
	struct PrimitiveType* p1;
	struct PrimitiveType* p2;
	struct Op* op;
};

static struct Type* infer_type_expr_primitive(struct ST *st, struct Expr2Types *e2t);

static struct Type* infer_type_expr_both_tparam(struct ST* st, struct TypeParam* tp1, struct Op* op, struct TypeParam* tp2);

static void typeinfer_err_fatal(char* opt_str){
    printf("%s\n", opt_str);
    printf("[Typeinference][Error] Fatal. (in typeinfer_expr.c). Exiting.\n");
    exit(1);
}

struct Type* infer_type_expr(struct ST *st, struct Expr *expr) {

	if(expr->term2 == NULL)
		{ return infer_type_unopterm(st, expr->term1); }
	
	struct UnOpTerm* t1 = expr->term1;
	struct UnOpTerm* t2 = expr->term2;
	struct Op* op = expr->op;

	struct Type* type1 = infer_type_unopterm(st, t1);
	struct Type* type2 = infer_type_unopterm(st, t2);

	if(type1->type_param != NULL && type2->type_param != NULL){
        return infer_type_expr_both_tparam(st, type1->type_param, expr->op, type2->type_param);
	}
	
	struct BasicType* btw1 = type1->basic_type;
	struct BasicType* btw2 = type2->basic_type;
	
	if(btw1 == NULL || btw2 == NULL){
	    typeinfer_err_fatal(str_expr(expr));
	}
	
	struct SimpleType* st1 = btw1->simple_type;
	struct SimpleType* st2 = btw2->simple_type;
	
	if(st1 == NULL || st2 == NULL){
	    typeinfer_err_fatal(str_expr(expr));
	}
	
	bool p1 = st1->primitive_type != NULL;
	bool p2 = st2->primitive_type != NULL;
	
	if(!p1 || !p2){
	    typeinfer_err_fatal(str_expr(expr));
	}
	
	struct Expr2Types e2t = {
		.p1 = st1->primitive_type,
		.p2 = st2->primitive_type,
		.op = op
	};
	
	return infer_type_expr_primitive(st, &e2t);
}

static struct Type* infer_type_expr_both_tparam(struct ST* st, struct TypeParam* tp1, struct Op* op, struct TypeParam* tp2){

    bool same_type = tp1->index == tp2->index;

    if(!same_type){
        typeinfer_err_fatal(str_op(op));
    }

    if(op->is_relational)
        { return typeFromStrPrimitive(st, "bool"); }

    if(op->is_logical)
        { return typeFromStrPrimitive(st, "bool"); }

    if(op->is_bitwise)
        { return typeFromStrPrimitive(st, "int"); }

    typeinfer_err_fatal(str_op(op));
    return NULL;
}

static struct Type *infer_type_expr_primitive(struct ST *st, struct Expr2Types *e2t) {

	struct PrimitiveType* p1 = e2t->p1;
	struct PrimitiveType* p2 = e2t->p2;
	struct Op* op = e2t->op;
	
	if(op->is_relational)
		{ return typeFromStrPrimitive(st, "bool"); }
	
	if(op->is_logical)
		{ return typeFromStrPrimitive(st, "bool"); }
		
	const bool i1 = p1->is_int_type;
	const bool i2 = p2->is_int_type;
	
	const bool c1 = p1->is_char_type;
	const bool c2 = p2->is_char_type;

	
	if(op->is_arithmetic){
		
		if(i1 && i2)
			{ return typeFromStrPrimitive(st, "int"); }
	
		if(c1 && c2)
			{ return typeFromStrPrimitive(st, "char"); }

		if((c1 && i2) || (i1 && c2))
			{ return typeFromStrPrimitive(st, "char"); }
	}
	
	if(op->is_bitwise)
		{ return typeFromStrPrimitive(st, "int"); }

	//if we cannot figure out this thing,
	//we assume it is int. this is just typeinference so
	//no need to typecheck here.
	return typeFromStrPrimitive(st, "int");
}
