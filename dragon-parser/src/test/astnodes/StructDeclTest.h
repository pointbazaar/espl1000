#ifndef STRUCTDECLTEST
#define STRUCTDECLTEST

int structdecl_test_can_parse_empty_struct_decl();

int structdecl_test_will_not_parse_invalid_typename_for_struct();

int structdecl_test_rejects_struct_with_subroutine_type();

int structdecl_test_can_parse_struct_with_1_member();

int structdecl_test_can_parse_struct_with_2_members();

#endif