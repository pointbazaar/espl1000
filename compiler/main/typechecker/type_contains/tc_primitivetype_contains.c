#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"

#include "tc_type_contains.h"

bool tc_primitivetype_contains(struct PrimitiveType* expect, struct SimpleType* actual) {

	if (actual->primitive_type == NULL) {
		return false;
	}

	struct PrimitiveType* actual_pt = actual->primitive_type;

	if (expect->is_bool_type && actual_pt->is_bool_type) { return true; }
	if (expect->is_char_type && actual_pt->is_char_type) { return true; }
	if (expect->is_int_type && actual_pt->is_int_type) {
		enum INTTYPE it_e = expect->int_type;
		enum INTTYPE a = actual_pt->int_type;

		switch (it_e) {

			case INT8: return a == INT8;
			case INT16: return a == INT8 || a == INT16;
			case INT:
			case INT32: return a == INT8 || a == INT16 || a == INT32 || a == INT;
			case INT64: return a == INT8 || a == INT16 || a == INT32 || a == INT64 || a == INT;

			case UINT8: return a == UINT8;
			case UINT16: return a == UINT8 || a == UINT16;
			case UINT:
			case UINT32: return a == UINT8 || a == UINT16 || a == UINT32 || a == UINT;
			case UINT64: return a == UINT8 || a == UINT16 || a == UINT32 || a == UINT64 || a == UINT;

			default: break;
		}
	}
	return false;
}
