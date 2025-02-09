#include <stdio.h>
#include <stdlib.h>

#include "parser/main/util/parse_astnode.h"

#include "types/Type.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"
#include "ast/util/copy_ast.h"

struct LocalVarDeclStmt* makeLocalVarDeclStmt(struct TokenList* tokens) {

	struct LocalVarDeclStmt* res = make(LocalVarDeclStmt);
	struct TokenList* copy = list_copy(tokens);

	parse_astnode(copy, &(res->super));

	if (!list_expect(copy, KEYWORD_LOCAL)) {
		goto err_no_type;
	}

	res->type = makeType2(copy);
	if (res->type == NULL) {
		fprintf(stderr, "expected Type, got %s\n", list_code(copy));
		goto err_no_type;
	}

	struct Token* token = list_head(copy);
	if (token->kind != ID) {
		fprintf(stderr, "expected id, got %s\n", list_code(copy));
		goto err_no_id;
	}

	asprintf(&(res->name), "%s", token->value_ptr);
	list_consume(copy, 1);

	if (!list_expect(copy, SEMICOLON)) {
		fprintf(stderr, "expected ';', got %s\n", list_code(copy));
		goto err_no_semic;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;

err_no_semic:
	free(res->name);
err_no_id:
	free_type(res->type);
err_no_type:
	free(res);
	freeTokenListShallow(copy);
	return NULL;
}
