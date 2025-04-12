#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Namespace.h"
#include "subr/Method.h"
#include "struct/StructDecl.h"
#include "EnumDecl.h"

#include "ast/util/free_ast.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

// @returns false on error
static bool ns_parse_methods(struct Namespace* res, struct TokenList* copy);
// @returns false on error
static bool ns_parse_structs(struct Namespace* res, struct TokenList* copy);
// @returns false on error
static bool ns_parse_enums(struct Namespace* res, struct TokenList* copy);

// @returns false on error
bool ns_parse_passthrough_includes(struct Namespace* p_namespace, struct TokenList* p_list);

struct Namespace* makeNamespace(struct TokenList* tokens, char* name) {

	struct Namespace* res = make(Namespace);

	if (!res) {
		return NULL;
	}

	//valgrind will complain about uninitialized bytes otherwise
	memset(res, 0, sizeof(struct Namespace));

	res->count_methods = 0;
	res->count_structs = 0;
	res->count_enums = 0;

	const uint16_t INITIAL_CAPACITY = 5;
	res->capacity_methods = INITIAL_CAPACITY;
	res->capacity_structs = INITIAL_CAPACITY;
	res->capacity_enums = INITIAL_CAPACITY;

	res->methods = malloc(sizeof(struct Method*) * res->capacity_methods);
	if (!res->methods) {
		goto error;
	}
	res->structs = malloc(sizeof(struct StructDecl*) * res->capacity_structs);
	if (!res->structs) {
		goto error_res_methods;
	}
	res->enums = malloc(sizeof(struct EnumDecl*) * res->capacity_enums);
	if (!res->enums) {
		goto error_res_enums;
	}
	res->src_path = malloc(sizeof(char) * (strlen(name) + 3 + 1));
	if (!res->src_path) {
		goto error_res_structs;
	}
	res->token_path = malloc(sizeof(char) * (strlen(list_rel_path(tokens)) + 1));
	if (!res->token_path) {
		goto error_res_src_path;
	}

	sprintf(res->src_path, "%s.dg", name);
	strcpy(res->token_path, list_rel_path(tokens));

	asprintf(&(res->name), "%s", name);

	struct TokenList* copy = list_copy(tokens);

	if (!ns_parse_passthrough_includes(res, copy)) {
		goto error_res_token_path;
	}

	if (!ns_parse_enums(res, copy)) {
		goto error_res_parse_enums;
	}

	if (!ns_parse_structs(res, copy)) {
		goto error_res_parse_structs;
	}

	if (!ns_parse_methods(res, copy)) {
		goto error_res_parse_structs;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;

error_res_parse_structs:
	for (int i = 0; i < res->count_includes; i++) {
		free(res->includes[i]);
	}
	free(res->includes);
error_res_parse_enums:
	for (int i = 0; i < res->count_enums; i++) {
		free(res->enums[i]->name);
		free(res->enums[i]);
	}
error_res_token_path:
	free(res->token_path);
error_res_src_path:
	free(res->src_path);
error_res_name:
	free(res->name);
error_res_enums:
	free(res->enums);
error_res_structs:
	free(res->structs);
error_res_methods:
	free(res->methods);
error:
	freeTokenListShallow(copy);
	free(res);
	return NULL;
}

static bool ns_parse_enums(struct Namespace* res, struct TokenList* copy) {

	struct Token* next = list_head(copy);

	while (next->kind == KEYWORD_ENUM) {

		if ((res->count_enums + 1) >= res->capacity_enums) {
			res->capacity_enums *= 2;
			struct EnumDecl** new_enums = realloc(res->enums, sizeof(struct EnumDecl*) * res->capacity_enums);

			if (!new_enums) {
				return false;
			}

			res->enums = new_enums;
		}

		res->enums[res->count_enums] = makeEnumDecl(copy);
		if (!res->enums[res->count_enums]) {
			return false;
		}
		res->count_enums++;
		next = list_head(copy);
	}

	return true;
}

bool ns_parse_passthrough_includes(struct Namespace* res, struct TokenList* copy) {

	if (list_size(copy) == 0) { return true; }

	struct Token* next = list_head(copy);

	//speculate that it won't be more than 100
	uint8_t includes_cap = 100;
	res->includes = malloc(sizeof(void*) * includes_cap);
	if (!res->includes) {
		return false;
	}
	res->count_includes = 0;

	while (next->kind == INCLUDE_DECL) {
		char* string = malloc(strlen(next->value_ptr) + 1);
		if (!string) {
			goto error;
		}
		strcpy(string, next->value_ptr);

		res->includes[res->count_includes++] = string;
		list_consume(copy, 1);

		next = list_head(copy);
	}

	return true;

error:
	for (int i = 0; i < res->count_includes; i++) {
		free(res->includes[i]);
	}
	free(res->includes);
	return false;
}

static bool ns_parse_methods(struct Namespace* res, struct TokenList* copy) {

	if (list_size(copy) == 0) { return true; }

	struct Token* next = list_head_without_annotations(copy);

	while (next->kind == FN) {
		struct Method* m = makeMethod(copy);
		if (m == NULL) {
			printf("parsing error, expected a method, but got:\n");
			list_print(copy);

			return false;
		}

		res->methods[res->count_methods] = m;
		res->count_methods++;

		if (res->count_methods >= res->capacity_methods) {
			res->capacity_methods *= 2;
			res->methods = realloc(res->methods, sizeof(struct Method*) * (res->capacity_methods));
		}

		if (list_size(copy) > 0) {
			next = list_head_without_annotations(copy);
		} else {
			break;
		}
	}

	return true;
}
static bool ns_parse_structs(struct Namespace* res, struct TokenList* copy) {

	if (list_size(copy) == 0) { return true; }

	struct Token* next = list_head_without_annotations(copy);

	while (next->kind == STRUCT) {

		struct StructDecl* sd = makeStructDecl(copy);
		if (sd == NULL) {
			fprintf(stderr, "parsing error, expected a struct, but got %s\n", list_code(copy));
			return false;
		}

		res->structs[res->count_structs] = sd;
		res->count_structs++;

		res->structs = realloc(res->structs, sizeof(struct StructDecl*) * (res->count_structs + 1));

		if (list_size(copy) > 0) {
			next = list_head_without_annotations(copy);
		} else {
			break;
		}
	}

	return true;
}
