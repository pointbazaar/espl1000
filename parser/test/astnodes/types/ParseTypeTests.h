#pragma once

#include <stdbool.h>

int basictype_test_type_parsing_simple_type();

int simpletype_test_typenode_parsing();
int simpletype_test_typenode_parsing_fails();
int simpletype_test_typenode_parsing_anytype();
int simpletype_test_generic();

int structtype_test();
int structtype_test_type_param();
int structtype_test_generic();

int subrtype_test_typename();
int subrtype_test_typename_subroutine_return_type();
int subrtype_test_subroutine_type_parsing_subroutine_with_side_effects();
int subrtype_test_subroutine_type_parsing_subroutine_without_side_effects();

int pointertype_test_type_parsing();
