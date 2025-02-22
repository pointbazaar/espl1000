#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "types/Type.h"
#include "../Identifier.h"
#include "DeclArg.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

static struct MethodDecl* initMethodDecl();
static bool tryParseArgList(struct MethodDecl* res, struct TokenList* copy);

struct MethodDecl* makeMethodDecl(struct TokenList* tokens) {

	struct MethodDecl* res = initMethodDecl();
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	if (!list_expect(copy, FN)) {
		//as a subroutine is parsed deterministically (we know to parse a subroutine by the 'fn' keyword),
		//give a little parse error message
		printf("expected 'fn', but was: %s\n", list_code(copy));
		free(res);
		return NULL;
	}

	struct Id* id = makeIdentifier(copy);
	if (id == NULL) {
		printf("expected method name, but was: %s\n", list_code(copy));
		free(res);
		return NULL;
	}

	asprintf(&(res->name), "%s", id->identifier);
	free_identifier(id);

	if (!tryParseArgList(res, copy)) {
		free(res);
		return NULL;
	}

	{
		struct Token* tk = list_head(copy);
		if (tk->kind != ARROW_NO_SIDE_EFFECT && tk->kind != ARROW_SIDE_EFFECT) {
			free(res);
			freeTokenListShallow(copy);
			return NULL;
		}

		res->has_side_effects = (tk->kind == ARROW_SIDE_EFFECT);

		list_consume(copy, 1);
	}

	res->return_type = makeType2(copy);
	if (res->return_type == NULL) {
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

static bool tryParseArgList(struct MethodDecl* res, struct TokenList* copy) {

	if (!list_expect(copy, LPARENS)) {
		return false;
	}

	//while there is no ')' up, continue parsing arguments
	struct Token* next = list_head(copy);

	if (next == NULL) {
		return false;
	}

	while (next->kind != RPARENS) {
		if (res->count_args > 0) {
			if (!list_expect(copy, COMMA)) {
				return false;
			}
		}

		struct DeclArg* da = makeDeclArg(copy);
		if (da == NULL) {
			return false;
		}
		res->args[res->count_args] = da;
		res->count_args++;

		res->args = realloc(res->args, sizeof(struct DeclArg*) * (res->count_args + 1));

		next = list_head(copy);
		if (next == NULL) {
			freeTokenListShallow(copy);
			return false;
		}
	}

	if (!list_expect(copy, RPARENS)) {
		freeTokenListShallow(copy);
		return false;
	}

	return true;
}

static struct MethodDecl* initMethodDecl() {

	struct MethodDecl* res = make(MethodDecl);

	if (!res) {
		return NULL;
	}

	res->is_public = true;
	res->has_side_effects = true;

	res->count_args = 0;
	res->args = malloc(sizeof(struct DeclArg*) * 1);

	if (!res->args) {
		free(res);
		return NULL;
	}

	return res;
}
