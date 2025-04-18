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
	enum OP op;
};

static struct Type* infer_type_expr_primitive(struct ST* st, struct Expr2Types* e2t);

static struct Type* infer_type_expr_both_tparam(struct ST* st, struct TypeParam* tp1, enum OP op, struct TypeParam* tp2);

static struct Type* infer_type_expr_ptr_arithmetic(struct ST* st, struct Type* t1, struct Type* t2, enum OP op);
static struct Type* infer_type_expr_array_arithmetic(struct ST* st, struct Type* t1, struct Type* t2, enum OP op);

static void typeinfer_err_fatal(char* opt_str) {
	fprintf(stderr, "%s\n", opt_str);
	fprintf(stderr, "[Typeinference][Error] Fatal. (in typeinfer_expr.c).\n");
}

struct Type* infer_type_expr(struct ST* st, struct Expr* expr) {

	if (expr->term2 == NULL) { return infer_type_unopterm(st, expr->term1); }

	struct UnOpTerm* t1 = expr->term1;
	struct UnOpTerm* t2 = expr->term2;
	enum OP op = expr->op;

	struct Type* type1 = infer_type_unopterm(st, t1);
	struct Type* type2 = infer_type_unopterm(st, t2);

	if (type1->type_param != NULL && type2->type_param != NULL) {
		return infer_type_expr_both_tparam(st, type1->type_param, expr->op, type2->type_param);
	}

	if (type1->pointer_type) {
		return infer_type_expr_ptr_arithmetic(st, type1, type2, op);
	}
	if (type2->pointer_type) {
		return infer_type_expr_ptr_arithmetic(st, type2, type1, op);
	}
	if (type1->array_type) {
		return infer_type_expr_array_arithmetic(st, type1, type2, op);
	}
	if (type2->array_type) {
		return infer_type_expr_array_arithmetic(st, type2, type1, op);
	}

	struct BasicType* btw1 = type1->basic_type;
	struct BasicType* btw2 = type2->basic_type;

	if (btw1 == NULL || btw2 == NULL) {
		typeinfer_err_fatal(str_expr(expr));
		return NULL;
	}

	struct SimpleType* st1 = btw1->simple_type;
	struct SimpleType* st2 = btw2->simple_type;

	if (st1 == NULL || st2 == NULL) {
		typeinfer_err_fatal(str_expr(expr));
		return NULL;
	}

	bool p1 = st1->primitive_type != NULL;
	bool p2 = st2->primitive_type != NULL;

	if (!p1 || !p2) {
		typeinfer_err_fatal(str_expr(expr));
		return NULL;
	}

	struct Expr2Types e2t = {
	    .p1 = st1->primitive_type,
	    .p2 = st2->primitive_type,
	    .op = op};

	return infer_type_expr_primitive(st, &e2t);
}

static struct Type* infer_type_expr_ptr_arithmetic(struct ST* st, struct Type* t1, struct Type* t2, enum OP op) {

	struct PointerType* pt = t1->pointer_type;

	if (!pt) {
		typeinfer_err_fatal("pointer arithmetic: not a pointer");
		return NULL;
	}

	if (t2->pointer_type) {
		if (op == OP_EQ || op == OP_NEQ || op == OP_GE || op == OP_GT || op == OP_LE || op == OP_LT) {
			return typeFromStrPrimitive(st, "bool");
		}
	}

	if (t2->basic_type && t2->basic_type->simple_type) {
		struct SimpleType* stype = t2->basic_type->simple_type;

		if (stype->primitive_type && stype->primitive_type->is_int_type) {

			if (op == OP_EQ || op == OP_NEQ || op == OP_GE || op == OP_GT || op == OP_LE || op == OP_LT) {

				return typeFromStrPrimitive(st, "bool");
			}

			return t1;
		}
	}

	char* str = str_type(t2);
	char* msg = NULL;

	if (!str) {
		return NULL;
	}

	if (asprintf(&msg, "pointer arithmetic: not an integer type: %s", str) == -1) {
		free(str);
		return NULL;
	}

	free(str);

	typeinfer_err_fatal(msg);

	free(msg);

	return NULL;
}

static struct Type* infer_type_expr_array_arithmetic(struct ST* st, struct Type* t1, struct Type* t2, enum OP op) {

	struct ArrayType* pt = t1->array_type;

	if (!pt) {
		typeinfer_err_fatal("pointer arithmetic: not a pointer");
		return NULL;
	}

	if (t2->pointer_type) {
		if (op == OP_EQ || op == OP_NEQ || op == OP_GE || op == OP_GT || op == OP_LE || op == OP_LT) {
			return typeFromStrPrimitive(st, "bool");
		}
	}

	if (t2->basic_type && t2->basic_type->simple_type) {
		struct SimpleType* stype = t2->basic_type->simple_type;

		if (stype->primitive_type && stype->primitive_type->is_int_type) {

			if (op == OP_EQ || op == OP_NEQ || op == OP_GE || op == OP_GT || op == OP_LE || op == OP_LT) {

				return typeFromStrPrimitive(st, "bool");
			}

			return t1;
		}
	}

	char* str = str_type(t2);
	char* msg = NULL;

	if (!str) {
		return NULL;
	}

	if (asprintf(&msg, "array arithmetic: not an integer type: %s", str) == -1) {
		free(str);
		return NULL;
	}

	free(str);

	typeinfer_err_fatal(msg);

	free(msg);

	return NULL;
}

static struct Type* infer_type_expr_both_tparam(struct ST* st, struct TypeParam* tp1, enum OP op, struct TypeParam* tp2) {

	bool same_type = tp1->index == tp2->index;

	if (!same_type) {
		typeinfer_err_fatal(str_op(op));
		return NULL;
	}

	if (op == OP_EQ || op == OP_NEQ || op == OP_GE || op == OP_GT || op == OP_LE || op == OP_LT) { return typeFromStrPrimitive(st, "bool"); }

	if (op == OP_AND || op == OP_OR || op == OP_XOR) { return typeFromStrPrimitive(st, "bool"); }

	if (op == OP_SHIFT_LEFT || op == OP_SHIFT_RIGHT) { return typeFromStrPrimitive(st, "int"); }

	typeinfer_err_fatal(str_op(op));
	return NULL;
}

static struct Type* infer_type_expr_primitive(struct ST* st, struct Expr2Types* e2t) {

	struct PrimitiveType* p1 = e2t->p1;
	struct PrimitiveType* p2 = e2t->p2;
	enum OP op = e2t->op;

	if (op == OP_EQ || op == OP_NEQ || op == OP_GE || op == OP_GT || op == OP_LE || op == OP_LT) { return typeFromStrPrimitive(st, "bool"); }

	const bool i1 = p1->is_int_type;
	const bool i2 = p2->is_int_type;

	const bool c1 = p1->is_char_type;
	const bool c2 = p2->is_char_type;

	const bool b1 = p1->is_bool_type;
	const bool b2 = p2->is_bool_type;

	if (op == OP_AND || op == OP_OR || op == OP_XOR) {

		if (b1 && b2)
			return typeFromStrPrimitive(st, "bool");

		if (i1 && i2)
			return typeFromStrPrimitive(st, "int");
	}

	if (op == OP_PLUS || op == OP_MINUS || op == OP_MULTIPLY) {

		if (i1 && i2) { return typeFromStrPrimitive(st, "int"); }

		if (c1 && c2) { return typeFromStrPrimitive(st, "char"); }

		if ((c1 && i2) || (i1 && c2)) { return typeFromStrPrimitive(st, "char"); }
	}

	if (op == OP_SHIFT_LEFT || op == OP_SHIFT_RIGHT) { return typeFromStrPrimitive(st, "int"); }

	//if we cannot figure out this thing,
	//we assume it is int. this is just typeinference so
	//no need to typecheck here.
	return typeFromStrPrimitive(st, "int");
}
