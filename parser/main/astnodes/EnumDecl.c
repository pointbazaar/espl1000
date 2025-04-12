#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser/main/util/parse_astnode.h"

#include "EnumDecl.h"
#include "EnumMember.h"
#include "types/SimpleType.h"
#include "types/StructType.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct EnumDecl* makeEnumDecl(struct TokenList* tokens) {

	if (list_size(tokens) < 2) {
		return NULL;
	}

	struct TokenList* copy = list_copy(tokens);

	struct EnumDecl* res = make(EnumDecl);
	if (!res) {
		return NULL;
	}

	parse_astnode(copy, &(res->super));

	res->members = malloc(sizeof(struct EnumMember*) * 1);

	if (!res->members) {
		free(res);
		return NULL;
	}

	res->count_members = 0;

	struct Token* next = list_head(copy);

	if (next->kind != KEYWORD_ENUM) {
		fprintf(stderr, "parsing error, expected 'enum' , but got: %s\n", list_code(copy));
		return NULL;
	}
	list_consume(copy, 1);

	//read the enum name
	if (list_head(copy)->kind != TOKEN_ENUM_VALUE) {

		fprintf(stderr, "parsing error, expected enum value (all uppercase) , but got: %s\n", list_code(copy));
		return NULL;
	}
	res->name = strdup(list_head(copy)->value_ptr);
	list_consume(copy, 1);

	next = list_head(copy);
	if (next->kind != LCURLY) {
		fprintf(stderr, "parsing error, expected '{' , but got: %s\n", list_code(copy));
		return NULL;
	}
	list_consume(copy, 1);

	struct EnumMember* member;

	member = makeEnumMember(copy);
	while (member != NULL) {

		res->members[res->count_members] = member;
		res->count_members++;

		struct EnumMember** new_members = realloc(
		    res->members,
		    sizeof(struct EnumMember*) * (res->count_members + 1));

		if (!new_members) {
			goto exit_error;
		}

		res->members = new_members;

		if (list_head(copy)->kind != TOKEN_ENUM_VALUE) {
			break;
		}

		member = makeEnumMember(copy);
	}

	next = list_head(copy);
	if (next->kind != RCURLY) {
		fprintf(stderr, "parsing error, expected '}' , but got: %s\n", list_code(copy));
		goto exit_error;
	}
	list_consume(copy, 1);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;

exit_error:
	for (size_t i = 0; i < res->count_members; i++) {
		free(res->members[i]->name);
		free(res->members[i]);
	}
	free(res);
	freeTokenListShallow(copy);
	return NULL;
}
