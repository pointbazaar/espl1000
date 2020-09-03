#ifndef STRUCTDECLTEST
#define STRUCTDECLTEST

#include <stdbool.h>

int structdecl_test_can_parse_empty_struct_decl(bool debug);

int structdecl_test_will_not_parse_invalid_typename_for_struct(bool debug);

int structdecl_test_rejects_struct_with_subroutine_type(bool debug);

int structdecl_test_can_parse_struct_with_1_member(bool debug);

int structdecl_test_can_parse_struct_with_2_members(bool debug);

#endif