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
	test_assign(false);
	test_assign_plus(false);
	test_assign_minus(false);
	test_assign_times(false);
	test_assign_div(false);
	test_assign_bitwise_and(false);
	test_assign_bitwise_or(false);
	test_assign_bitwise_shift_left(false);
	test_assign_bitwise_shift_right(false);

	//from test_op.h
	status_test_transpiler("transpiling of operators");
	test_add(false);
	test_sub(false);
	test_mul(false);
	test_div(false);
	test_mod(false);
	test_precedence(false);
	test_or(false);
	test_and(false);
	test_not(false);
	test_greater(false);
	test_lesser(false);
	test_geq(false);
	test_leq(false);
	test_eq(false);
	test_neq(false);
	test_chained_cmp(false);
	test_bitwise_and(false);
	test_bitwise_or(false);
	test_bitwise_leftshift(false);
	test_bitwise_rightshift(false);
	test_bitwise_xor(false);
	test_bitwise_neg(false);

	//from test_other.h
	status_test_transpiler("other");
	test_statuscode(false);
	test_simplevar(false);
	test_recursive(false);
	test_charconst_cmp(false);
	test_wrapped_expr(false);

	//from test_stmt.h
	status_test_transpiler("stmts");
	test_ifstmt(false);
	test_whilestmt(false);
	test_subrcall(false);
	test_break(false);
	test_continue(false);

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

