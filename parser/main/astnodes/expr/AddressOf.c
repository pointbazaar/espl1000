#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "Deref.h"
#include "Term.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"

struct AddressOf* makeAddresOf(struct Term* term) {

	struct AddressOf* res = make(AddressOf);

	res->super.line_num = term->super.line_num;
	res->super.annotations = 0;

	res->term = term;

	return res;
}

struct AddressOf* makeAddressOf2(struct TokenList* tokens) {

	struct AddressOf* res = make(AddressOf);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	if (!list_expect(copy, OPKEY_BITWISE_AND)) {
		goto error;
	}

	res->term = makeTerm(copy);
	if (res->term == NULL) {
		goto error;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;

error:
	freeTokenListShallow(copy);
	free(res);
	return NULL;
}
