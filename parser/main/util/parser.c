#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "Namespace.h"

#include "ast/util/free_ast.h"

#include "token/reader/token_reader.h"
#include "token/token/token.h"
#include "token/list/TokenList.h"

#include "parser/main/util/parser.h"

#define ERR_COULD_NOT_OPEN_FILE "[Parser] could not open file: %s\n"
#define ERR_FATAL "[Parser] Fatal.\n"
#define ERR_TOKENS_LEFT "[Parser] there were tokens left when parsing.\n"

static char* extract_namespace_name(char* filename_tokens);

struct AST* build_ast(char* tokensFile) {

	struct AST* ast = make(AST);

	ast->count_namespaces = 1;
	ast->namespaces = malloc(sizeof(struct Namespace*) * 1);
	if (!ast->namespaces) {
		return NULL;
	}
	ast->namespaces[0] = build_namespace(tokensFile);

	if (ast->namespaces[0] == NULL)
		return NULL;

	return ast;
}

struct Namespace* build_namespace(char* tokensFile) {

	FILE* file = fopen(tokensFile, "r");

	if (file == NULL) {
		fprintf(stderr, ERR_COULD_NOT_OPEN_FILE, tokensFile);
		return NULL;
	}

	//full buffering for performance
	setvbuf(file, NULL, _IOFBF, BUFSIZ);

	struct TokenList* tokens = read_tokens_from_tokens_file(file, tokensFile);

	if (!tokens) {
		return NULL;
	}

	fclose(file);

	//get just the namespace name from .FILENAME.dg.tokens
	char* ns_name = extract_namespace_name(tokensFile);
	if (!ns_name) {
		goto error;
	}

	struct Namespace* ns = makeNamespace(tokens, ns_name);

	free(ns_name);

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
	free(ns_name);
error:
	freeTokenList(tokens);
	return NULL;
}

static char* extract_namespace_name(char* filename_tokens) {

	//?.FILENAME.dg.tokens
	// ^        ^
	//dot1      dot2

	char* last = filename_tokens + strlen(filename_tokens) - 1;

	while (last[0] != '.') {
		last--;
	}
	last--;

	while (last[0] != '.') {
		last--;
	}
	char* dot2 = last;
	last--;

	while (last[0] != '.') {
		last--;
	}
	char* dot1 = last;
	last--;

	int l = (int)(dot2 - dot1);

	char* res = malloc(l + 1);
	if (!res) {
		return NULL;
	}
	memset(res, 0, l + 1);
	strncpy(res, dot1 + 1, l - 1);

	return res;
}
