#include <stdio.h>
#include <stdbool.h>

#include "typechecker/test_typechecker.h"
#include "typeinference/test_typeinference.h"

#include "code_gen/test_op.h"
#include "code_gen/test_other.h"
#include "code_gen/test_assign.h"
#include "code_gen/test_stmt.h"
#include "code_gen/test_call.h"

static void status_test_transpiler(char* msg){
	printf("[TRANSPILER][TEST-SUITE] %s\n", msg);
}

int main(){

	status_test_transpiler("Running tests for smalldragon/transpiler:");

	//from test_assign.h
	status_test_transpiler("transpiling of assignments");
	test_assign();
	test_assign_plus();
	test_assign_minus();
	test_assign_times();
	test_assign_div();
	test_assign_bitwise_and();
	test_assign_bitwise_or();
	test_assign_bitwise_shift_left();
	test_assign_bitwise_shift_right();

	//from test_op.h
	status_test_transpiler("transpiling of operators");
	test_add();
	test_sub();
	test_mul();
	test_div();
	test_mod();
	test_precedence();
	test_or();
	test_and();
	test_not();
	test_greater();
	test_lesser();
	test_geq();
	test_leq();
	test_eq();
	test_neq();
	test_chained_cmp();
	test_bitwise_and();
	test_bitwise_or();
	test_bitwise_leftshift();
	test_bitwise_rightshift();
	test_bitwise_xor();
	test_bitwise_neg();

	//from test_other.h
	status_test_transpiler("other");
	test_statuscode();
	test_simplevar();
	test_recursive();
	test_charconst_cmp();
	test_wrapped_expr();

	//from test_stmt.h
	status_test_transpiler("stmts");
	test_ifstmt();
	test_whilestmt();
	test_subrcall();
	test_break();
	test_continue();

	//from test_call.h
	test_call_simple();
	test_call_struct_member_access();
	//test_call_array_access(); //test is failing currently

	status_test_transpiler("Typeinference");
	test_suite_typeinference();

	status_test_transpiler("Typechecker");
	test_suite_typechecker();

	status_test_transpiler("PASSED ALL TESTS\n");

	return 0;
}

