#pragma once

#include <stdbool.h>
#include "ast/ast_declare.h"

//determines if the 'expect' type does contain the 'actual' type,
//e.g. uint32 contains uint16
//e.g. List<?T0> contains List<int>
bool tc_type_contains(struct Type* expect, struct Type* actual);

bool tc_basictype_contains(struct BasicType* expect, struct Type* actual);
bool tc_arraytype_contains(struct ArrayType* expect, struct ArrayType* actual);
bool tc_simpletype_contains(struct SimpleType* expect, struct Type* actual);
bool tc_subrtype_contains(struct SubrType* expect, struct SubrType* actual);
bool tc_structtype_contains(struct StructType* expect, struct StructType* actual);
bool tc_primitivetype_contains(struct PrimitiveType* expect, struct Type* actual);
bool tc_pointer_type_contains(struct PointerType* expect, struct Type* actual);
