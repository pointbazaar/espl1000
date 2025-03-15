#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"

#include "tc_type_contains.h"

bool tc_simpletype_contains(struct SimpleType* expect, struct BasicType* actual) {

	if (actual->simple_type == NULL) {
		return false;
	}

	struct SimpleType* actual_st = actual->simple_type;

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
		if (actual_st->primitive_type != NULL) {
			return tc_primitivetype_contains(expect->primitive_type, actual_st);
		}
		return false;
	}
	return false;
}
