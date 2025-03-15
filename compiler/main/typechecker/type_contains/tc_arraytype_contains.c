#include <stdbool.h>

#include "ast/ast.h"

#include "tc_type_contains.h"

bool tc_arraytype_contains(struct ArrayType* expect, struct ArrayType* actual) {
	return tc_type_contains(expect->element_type, actual->element_type);
}
