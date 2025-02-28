#include <string.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "UnOpTerm.h"

#include "Op.h"
#include "Term.h"
#include "AddressOf.h"
#include "Deref.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/token.h"

struct UnOpTerm* makeUnOpTerm(struct TokenList* tokens) {

	if (list_size(tokens) == 0) { return NULL; }

	struct UnOpTerm* res = make(UnOpTerm);

	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	struct Token* head = list_head(copy);

	if (head->kind == OPKEY_ARITHMETIC_MUL) {
		res->deref = makeDeref2(copy);
		if (!res->deref) {
			goto error;
		}
		goto end;
	}

	if (head->kind == OPKEY_BITWISE_AND) {
		res->address_of = makeAddressOf2(copy);
		if (!res->address_of) {
			goto error;
		}
		goto end;
	}

	res->op = makeOp(copy);
	//res->op may be OP_NONE, it is not a problem,
	//as the unary operator is optional

	if (res->op != OP_NONE) {

		//check if it is unary operator.
		//sometimes it inserts a space
		//so check for that case also
		if (
		    res->op != OP_MINUS && res->op != OP_NOT && res->op != OP_COMPLEMENT) {
			//the operator was not unary,
			//so we do not have an unary op term
			goto error;
		}
	}

	res->term = makeTerm(copy);

	if (res->term == NULL) {
		goto error;
	}
end:
	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
error:
	freeTokenListShallow(copy);
	free(res);
	return NULL;
}
