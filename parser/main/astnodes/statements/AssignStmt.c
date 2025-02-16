#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parser/main/util/parse_astnode.h"

#include "statements/AssignStmt.h"
#include "expr/Expr.h"
#include "var/Variable.h"
#include "types/Type.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"
#include "ast/util/copy_ast.h"
#include "expr/Op.h"

static int handle_assignment_operator(struct AssignStmt* res, int token_key, char* assign_op);
static struct UnOpTerm* convert_left(struct Variable* myvar);
static struct UnOpTerm* convert_right(struct Expr* expr);

struct AssignStmt* makeAssignStmt(struct TokenList* tokens) {

	struct AssignStmt* res = make(AssignStmt);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->opt_type = NULL;

	struct TokenList* copy2 = list_copy(copy);
	res->opt_type = makeType2(copy2);
	if (res->opt_type != NULL) {
		list_set(copy, copy2);
	}
	freeTokenListShallow(copy2);

	res->var = makeVariable(copy);
	if (res->var == NULL) {
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	struct Token* tkn_assign = list_head(copy);
	if (
	    tkn_assign->kind != ASSIGNOP_PLUS &&
	    tkn_assign->kind != ASSIGNOP_MINUS &&
	    tkn_assign->kind != ASSIGNOP_SHIFT_LEFT &&
	    tkn_assign->kind != ASSIGNOP_SHIFT_RIGHT &&
	    tkn_assign->kind != ASSIGNOP_BITWISE_AND &&
	    tkn_assign->kind != ASSIGNOP_BITWISE_OR &&
	    tkn_assign->kind != ASSIGNOP_SIMPLE) {
		free_variable(res->var);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	list_consume(copy, 1);

	res->expr = makeExpr(copy);
	if (res->expr == NULL) {
		free_variable(res->var);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	if (!list_expect(copy, SEMICOLON)) {
		free_expr(res->expr);
		free_variable(res->var);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	//handle the assignment operator,
	//can be [=, +=, -=, *=, ...]
	//(transform:  a ?= b  ->  a = a ? b)
	int status = handle_assignment_operator(res, tkn_assign->kind, tkn_assign->value_ptr);

	if (status != 0) {
		free_expr(res->expr);
		free_variable(res->var);
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

static struct UnOpTerm* convert_left(struct Variable* v) {

	struct Variable* myvar = copy_variable(v);

	struct Term* myterm1 = make(Term);
	myterm1->super = myvar->super;
	myterm1->kind = 6;
	myterm1->ptr.m6 = myvar;

	struct UnOpTerm* uop1 = make(UnOpTerm);
	uop1->super = myvar->super;
	uop1->op = OP_NONE;
	uop1->term = myterm1;
	return uop1;
}
static struct UnOpTerm* convert_right(struct Expr* expr) {

	struct UnOpTerm* uop2 = make(UnOpTerm);
	uop2->super = expr->super;

	struct Term* myterm2 = make(Term);
	myterm2->super = expr->super;
	myterm2->kind = 5;
	myterm2->ptr.m5 = expr;

	uop2->op = OP_NONE;
	uop2->term = myterm2;
	return uop2;
}

// @returns OP_NONE in error case
static enum OP assignop_to_op(int token_key) {

	switch (token_key) {
		case ASSIGNOP_PLUS:
			return OP_PLUS;
		case ASSIGNOP_MINUS:
			return OP_MINUS;
		case ASSIGNOP_SHIFT_LEFT:
			return OP_SHIFT_LEFT;
		case ASSIGNOP_SHIFT_RIGHT:
			return OP_SHIFT_RIGHT;
		case ASSIGNOP_BITWISE_AND:
			return OP_AND;
		case ASSIGNOP_BITWISE_OR:
			return OP_OR;
		default:
			fprintf(stderr, "%s: could not convert from assignop=%d, into op\n", __func__, token_key);
			return OP_NONE;
	}
}

static int handle_assignment_operator(struct AssignStmt* res, int token_key, char* assign_op) {

	//in case assign_op != "=",
	//we must transform
	// a ?= b -> a = a ? b

	if (token_key == ASSIGNOP_SIMPLE) {
		return 0;
	}

	enum OP myop = assignop_to_op(token_key);

	if (myop == OP_NONE) {
		return -1;
	}

	struct UnOpTerm* uop1 = convert_left(res->var);

	struct UnOpTerm* uop2 = convert_right(res->expr);

	struct Expr* expr = makeExpr_3(uop1, myop, uop2);

	res->expr = expr;

	return 0;
}
