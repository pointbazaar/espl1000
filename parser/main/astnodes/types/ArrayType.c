#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "ArrayType.h"
#include "Type.h"

#include "ast/ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

struct ArrayType* makeArrayType(struct Type* element_type) {

	struct ArrayType* res = make(ArrayType);

	res->super.line_num = element_type->super.line_num;
	res->super.annotations = 0;

	res->element_type = element_type;

	return res;
}

struct ArrayType* makeArrayType2(struct TokenList* tokens) {

	struct ArrayType* res = make(ArrayType);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	if (!list_expect(copy, LBRACKET)) {
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

	if (!list_expect(copy, RBRACKET)) {
		freeTokenListShallow(copy);
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}
