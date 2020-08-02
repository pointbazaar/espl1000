#ifndef SUBRTYPETEST
#define SUBRTYPETEST

#include <stdbool.h>

int subrtype_test_typename(bool debug);
int subrtype_test_typename_subroutine_return_type(bool debug);
int subrtype_test_subroutine_type_parsing_subroutine_with_side_effects(bool debug);
int subrtype_test_subroutine_type_parsing_subroutine_without_side_effects(bool debug);

#endif