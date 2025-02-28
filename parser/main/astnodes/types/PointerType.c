#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "PointerType.h"
#include "Type.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"

struct PointerType* makePointerType(struct Type* element_type) {

	struct PointerType* res = make(PointerType);

	res->super.line_num = element_type->super.line_num;
	res->super.annotations = 0;

	res->element_type = element_type;

	return res;
}

struct PointerType* makePointerType2(struct TokenList* tokens) {

	struct PointerType* res = make(PointerType);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	if (!list_expect(copy, OPKEY_ARITHMETIC_MUL)) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	res->element_type = makeType2(copy);
	if (res->element_type == NULL) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
