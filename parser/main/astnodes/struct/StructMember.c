#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser/main/util/parse_astnode.h"

#include "StructMember.h"
#include "../Identifier.h"
#include "types/Type.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct StructMember* initStructMember();
void beforeAbort(struct StructMember* m, struct TokenList* copy);
//-------------
struct StructMember* makeStructMember(struct TokenList* tokens) {

	struct StructMember* res = initStructMember();
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	if ((res->type = makeType2(copy)) == NULL) {

		beforeAbort(res, copy);
		return NULL;
	}

	struct Id* id = makeIdentifier(copy);

	if (id == NULL) {
		beforeAbort(res, copy);
		return NULL;
	}

	//expect ';'
	struct Token* next = list_head(copy);
	if (next->kind != SEMICOLON) {
		beforeAbort(res, copy);
		return NULL;
	}
	list_consume(copy, 1);

	strncpy(res->name, id->identifier, DEFAULT_STR_SIZE);
	free_identifier(id);

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct StructMember* initStructMember() {

	struct StructMember* res = make(StructMember);
	res->type = NULL;

	return res;
}

void beforeAbort(struct StructMember* m, struct TokenList* copy) {

	if (m->type != NULL) {
		free_type(m->type);
	}
	free(m);

	freeTokenListShallow(copy);
}
