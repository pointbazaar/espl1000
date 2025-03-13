#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

#include "Namespace.h"

#include "ast/util/free_ast.h"

#include "token/reader/token_reader.h"
#include "token/token/token.h"
#include "token/list/TokenList.h"

#include "parser/main/util/parser.h"

#define ERR_COULD_NOT_OPEN_FILE "[Parser] could not open file: %s\n"
#define ERR_FATAL "[Parser] Fatal.\n"
#define ERR_TOKENS_LEFT "[Parser] there were tokens left when parsing.\n"

struct AST* build_ast(int tokensFd, char* filename_display) {

	struct AST* ast = make(AST);

	ast->count_namespaces = 1;
	ast->namespaces = malloc(sizeof(struct Namespace*) * 1);
	if (!ast->namespaces) {
		return NULL;
	}
	ast->namespaces[0] = build_namespace(tokensFd, filename_display);

	if (ast->namespaces[0] == NULL)
		return NULL;

	return ast;
}

struct Namespace* build_namespace(int tokensFd, char* filename_display) {

	if (filename_display == NULL) {
		return NULL;
	}

	if (tokensFd < 0) {
		fprintf(stderr, "could not open fd %d", tokensFd);
		return NULL;
	}

	struct TokenList* tokens = read_tokens_from_tokens_file(tokensFd, filename_display);

	if (!tokens) {
		return NULL;
	}

	close(tokensFd);

	struct Namespace* ns = makeNamespace(tokens, filename_display);

	if (!ns) {
		goto error_namespace;
	}

	if (list_size(tokens) > 0) {
		fprintf(stderr, ERR_TOKENS_LEFT);
		list_print(tokens);
		goto error_tokens_left;
	}

	freeTokenList(tokens);

	return ns;

error_tokens_left:
	free_namespace(ns);
error_namespace:
error:
	freeTokenList(tokens);
	return NULL;
}
