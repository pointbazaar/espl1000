#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"

#include "equals_ast.h"

bool eq_type(struct Type* a, struct Type* b) {

	if (a == NULL || b == NULL) { return false; }

	if (a->basic_type != NULL) { return eq_basictype(a->basic_type, b->basic_type); }

	if (a->type_param != NULL) { return eq_typeparam(a->type_param, b->type_param); }

	if (a->array_type != NULL) { return eq_arraytype(a->array_type, b->array_type); }

	if (a->pointer_type != NULL) { return eq_pointertype(a->pointer_type, b->pointer_type); }

	if (a->is_anytype && b->is_anytype) {
		return true;
	}

	return false;
}

bool eq_subrtype(struct SubrType* a, struct SubrType* b) {

	if (a == NULL || b == NULL) { return false; }

	if (!eq_type(a->return_type, b->return_type)) { return false; }

	if (a->has_side_effects != b->has_side_effects) { return false; }

	if (a->count_arg_types != b->count_arg_types) { return false; }

	for (uint16_t i = 0; i < a->count_arg_types; i++) {

		if (!eq_type(a->arg_types[i], b->arg_types[i])) { return false; }
	}

	return true;
}

bool eq_typeparam(struct TypeParam* a, struct TypeParam* b) {

	if (a == NULL || b == NULL) { return false; }

	return a->index == b->index;
}

bool eq_simpletype(struct SimpleType* a, struct SimpleType* b) {

	if (a == NULL || b == NULL) { return false; }

	if (a->primitive_type != NULL) { return eq_primitivetype(a->primitive_type, b->primitive_type); }

	if (a->struct_type != NULL) { return eq_structtype(a->struct_type, b->struct_type); }

	return false;
}

bool eq_structtype(struct StructType* a, struct StructType* b) {

	if (a == NULL || b == NULL) { return false; }

	if (strcmp(a->type_name, b->type_name) != 0) { return false; }

	return true;
}

bool eq_primitivetype(struct PrimitiveType* a, struct PrimitiveType* b) {

	if (a == NULL || b == NULL) { return false; }

	if (a->is_char_type && b->is_char_type) { return true; }
	if (a->is_bool_type && b->is_bool_type) { return true; }

	if (a->is_int_type && b->is_int_type) {
		if (a->int_type == b->int_type) { return true; }
	}

	return false;
}

bool eq_basictype(struct BasicType* a, struct BasicType* b) {

	if (a == NULL || b == NULL) { return false; }

	if (a->simple_type != NULL) { return eq_simpletype(a->simple_type, b->simple_type); }

	if (a->subr_type != NULL) { return eq_subrtype(a->subr_type, b->subr_type); }

	return false;
}

bool eq_arraytype(struct ArrayType* a, struct ArrayType* b) {

	if (a == NULL || b == NULL) { return false; }

	return eq_type(a->element_type, b->element_type);
}

bool eq_pointertype(struct PointerType* a, struct PointerType* b) {

	if (a == NULL || b == NULL) { return false; }

	return eq_type(a->element_type, b->element_type);
}
