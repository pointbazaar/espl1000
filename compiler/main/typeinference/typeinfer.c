#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/str_ast.h"
#include "typeinference/typeinfer.h"

static char* ERR_NO_ARRAYTYPE = "Expected an ArrayType\n";

static void print_error(char* msg) {
	fprintf(stderr, "[Typeinference][Error] %s", msg);
}

struct Type* unwrap_indices(struct Type* t, uint32_t count) {

	assert(t);

	uint32_t index_count = count;

	struct Type* res = t;

	while (index_count > 0) {
		if (!(res = unwrap(res))) {
			return NULL;
		}
		index_count--;
	}

	return res;
}

struct Type* unwrap(struct Type* t) {

	if (t->array_type == NULL) {
		print_error(ERR_NO_ARRAYTYPE);
		char* s = str_type(t);
		fprintf(stderr, "instance: %s\n", s);
		free(s);
		return NULL;
	}

	return t->array_type->element_type;
}
