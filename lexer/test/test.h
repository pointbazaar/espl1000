#ifndef LEXERTEST
#define LEXERTEST

#include <stdbool.h>

void printt(char* test_name);

//test subroutines 
//(returning true if the test was passed, false otherwise)

void test_try_catch_throw_throws();

void test_plus_plus_minus_minus();

void test_can_see_line_with_semicolon();
void test_can_see_line_with_operators();
void test_lexes_return_statement_favorably();
void test_lexes_other_return_statement();
void test_lexes_float_constant();
void test_lexes_escaped_char();

// // SECTION: the tests from the individual token classes

void test_anytypetoken();

void test_true();
void test_false();

void test_char();

void test_float_1();
void test_float_2();
void test_float_3();

void test_identifier_1();
void test_identifier_2();

void test_int_1();
void test_int_2();
void test_int_3();
void test_hex();
void test_binconst();

void test_struct();
void test_return();

void test_string_1();

void test_typeidentifier();
void test_typeidentifier_primitive();

void test_typeparameter_1();

void test_comma();
void test_arrow();
void test_mixed_1();
void test_mixed_2();
void test_mixed_3();
void test_mixed_4();
void test_mixed_5();
void test_mixed_6();
void test_mixed_7();
void test_mixed_8();
void test_mixed_9();
void test_mixed_10();
void test_mixed_11();
void test_mixed_12();
void test_mixed_13();
void test_mixed_14();
void test_mixed_15();
void test_mixed_16();

void test_operators();
void test_operators_cmp();
void test_operators_logical();
void test_operators_bitwise();

void test_if_else();

void test_break();

void test_for();
void test_in();

void test_rangeop_1();
void test_rangeop_2();

void test_member_access();

void test_switch_case();

void test_assign_operators();

void test_brackets();


void test_single_line_comment();
void test_multi_line_comment();
#endif
