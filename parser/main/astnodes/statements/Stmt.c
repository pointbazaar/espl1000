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

	struct Token* first = list_head(copy);

	switch (first->kind) {

		case KEYWORD_BREAK:
			res->kind = STMT_KIND_BREAK;
			list_consume(copy, 2);
			break;

		case KEYWORD_CONTINUE:
			res->kind = STMT_KIND_CONTINUE;
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
	res->ptr.call = NULL;

	return res;
}

static bool stmt_make_local_var_decl_stmt(struct Stmt* res, struct TokenList* copy) {

	res->kind = STMT_KIND_LOCAL_VAR_DECL;
	res->ptr.local_var_decl_stmt = makeLocalVarDeclStmt(copy);
	if (res->ptr.local_var_decl_stmt == NULL) {
		fprintf(stderr, "expected local var decl stmt stmt, but was: %s\n", list_code(copy));

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_while(struct Stmt* res, struct TokenList* copy) {

	res->kind = STMT_KIND_WHILE;
	res->ptr.while_stmt = makeWhileStmt(copy);
	if (res->ptr.while_stmt == NULL) {
		fprintf(stderr, "expected while stmt, but was:\n");
		list_print(copy);

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_if(struct Stmt* res, struct TokenList* copy) {

	res->kind = STMT_KIND_IF;
	res->ptr.if_stmt = makeIfStmt(copy);
	if (res->ptr.if_stmt == NULL) {
		fprintf(stderr, "expected if stmt, but was:\n");
		list_print(copy);

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_return(struct Stmt* res, struct TokenList* copy) {

	res->kind = STMT_KIND_RETURN;
	res->ptr.return_stmt = makeRetStmt(copy);
	if (res->ptr.return_stmt == NULL) {
		fprintf(stderr, "expected return stmt, but was:\n");
		list_print(copy);

		freeTokenListShallow(copy);
		free(res);
		return false;
	}
	return true;
}

static bool stmt_make_for(struct Stmt* res, struct TokenList* copy) {

	res->kind = STMT_KIND_FOR;
	res->ptr.for_stmt = makeForStmt(copy);
	if (res->ptr.for_stmt == NULL) {
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

	res->kind = STMT_KIND_ASSIGN;
	res->ptr.assign_stmt = makeAssignStmt(copy);

	if (res->ptr.assign_stmt != NULL) {
		return true;
	}

	res->kind = STMT_KIND_CALL;
	res->ptr.call = makeCall(copy);
	if (res->ptr.call == NULL) {
		fprintf(stderr, "expected method call, but was:\n");
		list_print(copy);
		return false;
	}
	if (!list_expect(copy, SEMICOLON)) {
		fprintf(stderr, "expected ';', but was:\n");
		list_print(copy);
		free_call(res->ptr.call);
		return false;
	}

	return true;
}
