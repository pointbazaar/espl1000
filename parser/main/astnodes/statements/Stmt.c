#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "Stmt.h"
#include "WhileStmt.h"
#include "ForStmt.h"
#include "IfStmt.h"
#include "RetStmt.h"
#include "Call.h"
#include "AssignStmt.h"
#include "LocalVarDeclStmt.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

// --- private subroutines ---

static struct Stmt* initStmt();

static bool stmt_make_while(struct Stmt* res, struct TokenList* copy);
static bool stmt_make_if(struct Stmt* res, struct TokenList* copy);
static bool stmt_make_return(struct Stmt* res, struct TokenList* copy);
static bool stmt_make_for(struct Stmt* res, struct TokenList* copy);
static bool stmt_make_local_var_decl_stmt(struct Stmt* res, struct TokenList* copy);
static bool stmt_make_other(struct Stmt* res, struct TokenList* copy);
// ---------------------------

struct Stmt* makeStmt(struct TokenList* tokens) {

	if (list_size(tokens) == 0) { return NULL; }

	struct Stmt* res = initStmt();
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	res->kind = -1;
	res->is_break = false;
	res->is_continue = false;

	struct Token* first = list_head(copy);

	switch (first->kind) {

		case KEYWORD_BREAK:
			res->kind = 99;
			res->is_break = true;
			list_consume(copy, 2);
			break;

		case KEYWORD_CONTINUE:
			res->kind = 99;
			res->is_continue = true;
			list_consume(copy, 2);
			break;

		case FOR:
			stmt_make_for(res, copy);
			break;
		case WHILE:
			if (!stmt_make_while(res, copy)) {
				goto error;
			}
			break;
		case IF:
			if (!stmt_make_if(res, copy)) {
				goto error;
			}
			break;
		case RETURN:
			stmt_make_return(res, copy);
			break;
		case KEYWORD_LOCAL:
			if (!stmt_make_local_var_decl_stmt(res, copy)) {
				goto error;
			}
			break;

		default:
			if (!stmt_make_other(res, copy)) {
				goto error;
			}
			break;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;

error:
	freeTokenListShallow(copy);
	free(res);
	return NULL;
}

struct Stmt* initStmt() {

	struct Stmt* res = make(Stmt);

	res->kind = 0;
	res->ptr.m1 = NULL;

	return res;
}

static bool stmt_make_local_var_decl_stmt(struct Stmt* res, struct TokenList* copy) {

	res->kind = 10;
	res->ptr.m10 = makeLocalVarDeclStmt(copy);
	if (res->ptr.m10 == NULL) {
		fprintf(stderr, "expected local var decl stmt stmt, but was: %s\n", list_code(copy));

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_while(struct Stmt* res, struct TokenList* copy) {

	res->kind = 2;
	res->ptr.m2 = makeWhileStmt(copy);
	if (res->ptr.m2 == NULL) {
		fprintf(stderr, "expected while stmt, but was:\n");
		list_print(copy);

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_if(struct Stmt* res, struct TokenList* copy) {

	res->kind = 3;
	res->ptr.m3 = makeIfStmt(copy);
	if (res->ptr.m3 == NULL) {
		fprintf(stderr, "expected if stmt, but was:\n");
		list_print(copy);

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_return(struct Stmt* res, struct TokenList* copy) {

	res->kind = 4;
	res->ptr.m4 = makeRetStmt(copy);
	if (res->ptr.m4 == NULL) {
		fprintf(stderr, "expected return stmt, but was:\n");
		list_print(copy);

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_for(struct Stmt* res, struct TokenList* copy) {

	res->kind = 7;
	res->ptr.m7 = makeForStmt(copy);
	if (res->ptr.m7 == NULL) {
		fprintf(stderr, "expected for stmt, but was:\n");
		list_print(copy);

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_other(struct Stmt* res, struct TokenList* copy) {

	//i don't want 'let' statements
	//because it just messes up the code and is
	//just bloat to write.
	//but parsing should be straightforward
	//and mostly deterministic
	//because it gives good performance and
	//to give good error messages

	res->kind = 5;
	res->ptr.m5 = makeAssignStmt(copy);

	if (res->ptr.m5 != NULL) {
		return true;
	}

	res->kind = 1;
	res->ptr.m1 = makeCall(copy);
	if (res->ptr.m1 == NULL) {
		fprintf(stderr, "expected method call, but was:\n");
		list_print(copy);
		return false;
	}
	if (!list_expect(copy, SEMICOLON)) {
		fprintf(stderr, "expected ';', but was:\n");
		list_print(copy);
		free_call(res->ptr.m1);
		return false;
	}

	return true;
}
