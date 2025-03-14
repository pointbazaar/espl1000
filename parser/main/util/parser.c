#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

#include "Namespace.h"

#include "ast/util/free_ast.h"

#include "token/reader/token_reader.h"
#include "token/list/TokenList.h"

#include "parser/main/util/parser.h"

#define ERR_COULD_NOT_OPEN_FILE "[Parser] could not open file: %s\n"
#define ERR_FATAL "[Parser] Fatal.\n"
#define ERR_TOKENS_LEFT "[Parser] there were tokens left when parsing.\n"

struct AST* build_ast(struct TokenList* list, char* filename_display) {

	struct AST* ast = make(AST);

	ast->count_namespaces = 1;
	ast->namespaces = malloc(sizeof(struct Namespace*) * 1);
	if (!ast->namespaces) {
		return NULL;
	}
	ast->namespaces[0] = build_namespace(list, filename_display);

	if (ast->namespaces[0] == NULL)
		return NULL;

	return ast;
}

struct Namespace* build_namespace(struct TokenList* list, char* filename_display) {

	if (filename_display == NULL) {
		return NULL;
	}

	if (!list) {
		fprintf(stderr, "invalid token list\n");
		return NULL;
	}

	struct Namespace* ns = makeNamespace(list, filename_display);

	if (!ns) {
		goto error_namespace;
	}

	if (list_size(list) > 0) {
		fprintf(stderr, ERR_TOKENS_LEFT);
		list_print(list);
		goto error_tokens_left;
	}

	return ns;

error_tokens_left:
	free_namespace(ns);
error_namespace:
error:
	freeTokenList(list);
	return NULL;
}
