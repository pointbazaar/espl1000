#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"

#include "tc_type_contains.h"

static bool tc_simpletype_contains_simpletype(struct SimpleType* expect, struct Type* actual) {

	if (actual->basic_type == NULL) {
		return false;
	}

	if (actual->basic_type->simple_type == NULL) {
		return false;
	}

	struct SimpleType* actual_st = actual->basic_type->simple_type;

	if (expect->struct_type != NULL) {

		if (actual_st->struct_type != NULL) {
			return tc_structtype_contains(expect->struct_type, actual_st->struct_type);
		} else {
			//actual->primitive_type != NULL
			struct PrimitiveType* p = actual_st->primitive_type;

			//in case of int it's a pointer
			return p->is_int_type;
		}
	}

	if (expect->primitive_type != NULL) {
		return tc_primitivetype_contains(expect->primitive_type, actual);
	}
	return false;
}

static bool tc_simpletype_contains_basictype(struct SimpleType* expect, struct Type* actual) {

	if (actual->basic_type == NULL) {
		return false;
	}

	if (actual->basic_type->simple_type) {

		return tc_simpletype_contains_simpletype(expect, actual);
	}

	return false;
}

bool tc_simpletype_contains(struct SimpleType* expect, struct Type* actual) {

	if (actual->basic_type) {
		return tc_simpletype_contains_basictype(expect, actual);
	}

	if (expect->primitive_type) {
		return tc_primitivetype_contains(expect->primitive_type, actual);
	}

	return false;
}
