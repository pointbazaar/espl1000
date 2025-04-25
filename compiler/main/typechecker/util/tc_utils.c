#include <stdbool.h>
#include <string.h>

//AST Includes
#include "ast/ast.h"

//Typechecker Includes
#include "tc_utils.h"

// @returns NULL if not found
char* tc_get_underlying_struct_name(struct Type* t) {

	if (!t) {
		return NULL;
	}

	if (!t->basic_type && !t->pointer_type) {
		return NULL;
	}

	if (t->basic_type) {

		if (!t->basic_type->simple_type) {
			return NULL;
		}
		if (!t->basic_type->simple_type->struct_type) {
			return NULL;
		}
		return t->basic_type->simple_type->struct_type->type_name;
	}

	if (t->pointer_type) {
		return tc_get_underlying_struct_name(t->pointer_type->element_type);
	}

	return NULL;
}

bool is_primitive_type(struct Type* type) {

	if (type->basic_type == NULL) { return false; }
	if (type->basic_type->simple_type == NULL) { return false; }

	if (type->basic_type->simple_type->primitive_type == NULL) { return false; }

	return true;
}

bool is_integer_type(struct Type* type) {

	if (!is_primitive_type(type)) { return false; }

	return type->basic_type->simple_type->primitive_type->is_int_type;
}

bool is_bool_type(struct Type* type) {

	if (!is_primitive_type(type)) { return false; }

	return type->basic_type->simple_type->primitive_type->is_bool_type;
}

bool is_char_type(struct Type* type) {

	if (!is_primitive_type(type)) { return false; }

	return type->basic_type->simple_type->primitive_type->is_char_type;
}

bool is_pointer_type(struct Type* type) {

	return type->pointer_type != NULL;
}

uint32_t max_indices_allowed(struct Type* type) {

	if (type->array_type != NULL) {
		return 1 + max_indices_allowed(type->array_type->element_type);
	}

	return 0;
}
