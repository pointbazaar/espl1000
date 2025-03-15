#include <stdbool.h>
#include <string.h>

#include "ast/ast.h"

#include "tc_type_contains.h"

bool tc_structtype_contains(struct StructType* expect, struct StructType* actual) {

	if (strcmp(expect->type_name, actual->type_name) != 0) { return false; }

	return true;
}
