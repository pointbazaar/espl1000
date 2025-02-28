#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"
#include "ast/util/equals_ast.h"
#include "typechecker/util/tc_utils.h"

#include "tc_type_contains.h"

static bool tc_basictype_contains(struct BasicType* expect, struct BasicType* actual);
static bool tc_arraytype_contains(struct ArrayType* expect, struct ArrayType* actual);
static bool tc_simpletype_contains(struct SimpleType* expect, struct SimpleType* actual);
static bool tc_subrtype_contains(struct SubrType* expect, struct SubrType* actual);
static bool tc_structtype_contains(struct StructType* expect, struct StructType* actual);
static bool tc_primitivetype_contains(struct PrimitiveType* expect, struct PrimitiveType* actual);
static bool tc_pointer_type_contains(struct PointerType* expect, struct Type* actual);

bool tc_type_contains(struct Type* expect, struct Type* actual) {

	if (expect->pointer_type != NULL) {

		return tc_pointer_type_contains(expect->pointer_type, actual);
	}

	if (expect->basic_type != NULL && actual->basic_type != NULL) {

		return tc_basictype_contains(expect->basic_type, actual->basic_type);
	}

	if (expect->array_type != NULL) {

		if (actual->array_type != NULL) {

			return tc_arraytype_contains(expect->array_type, actual->array_type);
		} else {

			//allow integer assignments to array types
			if (actual->basic_type != NULL && actual->basic_type->simple_type != NULL && actual->basic_type->simple_type->primitive_type != NULL) {
				struct PrimitiveType* pt = actual->basic_type->simple_type->primitive_type;

				return pt->is_int_type;
			}

			return false;
		}
	}

	//so expect->m2 should be != NULL
	//meaning that anything on the right is ok, since it's a type parameter
	return expect->type_param != NULL;
}

static bool tc_basictype_contains(struct BasicType* expect, struct BasicType* actual) {
	if (expect->simple_type != NULL && actual->simple_type != NULL) {
		return tc_simpletype_contains(expect->simple_type, actual->simple_type);
	}
	if (expect->subr_type != NULL && actual->subr_type != NULL) {
		return tc_subrtype_contains(expect->subr_type, actual->subr_type);
	}
	return false;
}

static bool tc_arraytype_contains(struct ArrayType* expect, struct ArrayType* actual) {
	return tc_type_contains(expect->element_type, actual->element_type);
}

static bool tc_pointer_type_contains(struct PointerType* expect, struct Type* actual) {

	// integers can be used as pointers
	if (actual->pointer_type == NULL) {
		return is_integer_type(actual);
	}

	struct PointerType* apt = actual->pointer_type;

	// pointers of same underlying type can be assigned to each other
	return eq_type(expect->element_type, apt->element_type);
}

static bool tc_simpletype_contains(struct SimpleType* expect, struct SimpleType* actual) {
	if (expect->struct_type != NULL) {

		if (actual->struct_type != NULL) {
			return tc_structtype_contains(expect->struct_type, actual->struct_type);
		} else {
			//actual->primitive_type != NULL
			struct PrimitiveType* p = actual->primitive_type;

			//in case of int it's a pointer
			return p->is_int_type;
		}
	}
	if (expect->primitive_type != NULL && actual->primitive_type != NULL) {
		return tc_primitivetype_contains(expect->primitive_type, actual->primitive_type);
	}
	return false;
}

static bool tc_structtype_contains(struct StructType* expect, struct StructType* actual) {

	if (strcmp(expect->type_name, actual->type_name) != 0) { return false; }

	return true;
}

static bool tc_subrtype_contains(struct SubrType* expect, struct SubrType* actual) {

	if (!tc_type_contains(expect->return_type, actual->return_type)) { return false; }
	if (actual->has_side_effects && !expect->has_side_effects) { return false; }

	if (expect->count_arg_types != actual->count_arg_types) { return false; }

	for (int i = 0; i < expect->count_arg_types; ++i) {
		struct Type* ei = expect->arg_types[i];
		struct Type* ai = actual->arg_types[i];
		if (!tc_type_contains(ei, ai)) { return false; }
	}
	return true;
}

static bool tc_primitivetype_contains(struct PrimitiveType* expect, struct PrimitiveType* actual) {

	if (expect->is_bool_type && actual->is_bool_type) { return true; }
	if (expect->is_char_type && actual->is_char_type) { return true; }
	if (expect->is_int_type && actual->is_int_type) {
		enum INTTYPE it_e = expect->int_type;
		enum INTTYPE a = actual->int_type;

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
