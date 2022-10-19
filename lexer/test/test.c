#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "test.h"
#include "../../token/token/token.h"
#include "../../token/TokenKeys.h"
#include "lexer_test_utils.h"

static uint32_t test_num = 1;

void printt(char* test_name){
	printf("[Lexer][Test][#%02d] %s", test_num++, test_name);
}

int main(){

	printf("[Lexer] Running Tests... \n");

	test_plus_plus_minus_minus();

	test_can_see_line_with_semicolon();
	test_can_see_line_with_operators();
	test_lexes_return_statement_favorably();
	test_lexes_other_return_statement();

	test_identifier_1();
	test_identifier_2();

	test_struct();
	test_return();

	test_string_1();

	test_typeidentifier();
	test_typeidentifier_primitive();

	test_typeparameter_1();

	test_comma();
	test_arrow();
	
	//tests_const.c
	test_lexes_escaped_char();
	test_anytypetoken();
	test_true();
	test_false();
	test_char();
	test_int_1();
	test_int_2();
	test_int_3();
	test_hex();
	test_binconst();
	
	//tests_mixed.c
	test_mixed_1();
	test_mixed_2();
	test_mixed_3();
	test_mixed_4();
	test_mixed_5();
	test_mixed_6();
	test_mixed_7();
	test_mixed_8();
	test_mixed_9();
	test_mixed_10();
	test_mixed_11();
	test_mixed_12();
	test_mixed_13();
	test_mixed_14();
	test_mixed_15();
	test_mixed_16();
	//----------------

	//tests_operators.c
	test_operators();
	test_operators_cmp();
	test_operators_logical();
	test_operators_bitwise();
	test_assign_operators();
	//----------------

	//tests_keywords.c
	test_if_else();
	test_break();
	test_for();
	test_in();
	test_switch_case();
	//----------------
	
	//tests_comments.c
	test_single_line_comment();
	test_multi_line_comment();

	test_rangeop_1();
	test_rangeop_2();

	test_member_access();

	test_brackets();

	printf("[Lexer] All %i Tests Passed\n", test_num-1);

	return 0;
}
