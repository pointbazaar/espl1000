#ifndef LEXERTEST
#define LEXERTEST

#include <stdbool.h>

int test_all(bool debug);

bool assert(bool condition);

//test subroutines 
//(returning true if the test was passed, false otherwise)


//Tests from CharacterListTest.java are omitted 
//(they do not test stuff thats in this program)

//from LexerTest.java
bool test_can_see_line_with_semicolon();
bool test_can_see_line_with_operators();
bool test_lexes_return_statement_favorably();
bool test_lexes_other_return_statement();
bool test_lexes_float_constant();
bool test_lexes_escaped_char();

// // SECTION: the tests from the individual token classes

//from AccessModifierTokenTest.java
//ATTENTION: there are no important tests in there

//from AnyTypeTokenTest.java
bool test_anytypetoken();

//from ArrowTokenTest.java
//TODO

//from BoolConstantTokenTest.java
bool test_true();
bool test_false();

//from CharConstantTokenTest.java
bool test_char();

//from FloatNonNegativeTokenTest.java
bool test_float_1();
bool test_float_2();
bool test_float_3();

//from IdentifierTokenTest.java
bool test_identifier_1();
bool test_identifier_2();

//from IntegerNonNegativeTokenTest.java
bool test_int_1();
bool test_int_2();
bool test_int_3();

bool test_struct();
bool test_return();

//from StringConstantTokenTest.java
bool test_string_1();
bool test_string_2();

//from TypeIdentifierTokenTest.java
bool test_typeidentifier_simple();
bool test_typeidentifier_other();
bool test_typeidentifier_token();

//from TypeParameterIdentifierTokenTest.java
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

bool test_operators();

bool test_loop();
bool test_if_else();

bool test_break();

#endif
