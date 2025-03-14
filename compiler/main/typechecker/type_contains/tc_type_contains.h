#pragma once

#include <stdbool.h>
struct Type;

//determines if the 'expect' type does contain the 'actual' type,
//e.g. uint32 contains uint16
//e.g. List<?T0> contains List<int>
bool tc_type_contains(struct Type* expect, struct Type* actual);
