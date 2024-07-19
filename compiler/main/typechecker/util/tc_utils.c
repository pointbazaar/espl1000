#include <stdbool.h>
#include <string.h>

//AST Includes
#include "ast/ast.h"

//Typechecker Includes
#include "tc_utils.h"

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

uint32_t max_indices_allowed(struct Type* type) {

	if (type->array_type != NULL) {
		return 1 + max_indices_allowed(type->array_type->element_type);
	}

	return 0;
}
