#ifndef LEXERTEST
#define LEXERTEST

#include <stdbool.h>

struct Token** lex(char* source, bool debug);

int test_all(bool debug);

//test subroutines 
//(returning true if the test was passed, false otherwise)

void test_plus_plus_minus_minus();

bool test_can_see_line_with_semicolon();
bool test_can_see_line_with_operators();
bool test_lexes_return_statement_favorably();
bool test_lexes_other_return_statement();
bool test_lexes_float_constant();
bool test_lexes_escaped_char();

// // SECTION: the tests from the individual token classes

bool test_anytypetoken();

bool test_true();
bool test_false();

bool test_char();

bool test_float_1();
bool test_float_2();
bool test_float_3();

bool test_identifier_1();
bool test_identifier_2();

bool test_int_1();
bool test_int_2();
bool test_int_3();
bool test_hex();
bool test_binconst();

bool test_struct();
bool test_return();

bool test_string_1();
bool test_string_2();

bool test_typeidentifier();
bool test_typeidentifier_primitive();

bool test_typeparameter_1();

bool test_comma();
bool test_arrow();
bool test_mixed_1();
bool test_mixed_2();
bool test_mixed_3();
bool test_mixed_4();
bool test_mixed_5();
bool test_mixed_6();
bool test_mixed_7();
bool test_mixed_8();
bool test_mixed_9();
bool test_mixed_10();
bool test_mixed_11();
bool test_mixed_12();
bool test_mixed_13();
bool test_mixed_14();
bool test_mixed_15();
bool test_mixed_16();

bool test_operators();
bool test_operators_cmp();
bool test_operators_logical();
bool test_operators_bitwise();

bool test_loop();
bool test_if_else();

bool test_break();

bool test_for();
bool test_in();

bool test_rangeop_1();
bool test_rangeop_2();

bool test_member_access();

bool test_switch_case();

bool test_assign_operators();

bool test_brackets();
#endif
