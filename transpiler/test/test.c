#include <stdio.h>
#include <stdbool.h>

#include "typechecker/test_typechecker.h"
#include "typeinference/test_typeinference.h"
#include "code_gen/test_op.h"

#include "code_gen/test_other.h"
#include "code_gen/test_assign.h"
#include "code_gen/test_stmt.h"

#include "test.h"

static void status_test_transpiler(char* msg){
	printf("[TRANSPILER][TEST-SUITE] %s\n", msg);
}

int main(){
	transpiler_test_all(false);
	return 0;
}

int transpiler_test_all(bool debug){

	status_test_transpiler("Running tests for smalldragon/transpiler:");
	
	//from test_assign.h
	status_test_transpiler("transpiling of assignments");
	test_assign(debug);
	test_assign_plus(debug);
	test_assign_minus(debug);
	test_assign_times(debug);
	test_assign_div(debug);
	test_assign_bitwise_and(debug);
	test_assign_bitwise_or(debug);
	test_assign_bitwise_shift_left(debug);
	test_assign_bitwise_shift_right(debug);
	
	//from test_op.h
	status_test_transpiler("transpiling of operators");
	test_add(debug);
	test_sub(debug);
	test_mul(debug);
	test_div(debug);
	test_mod(debug);
	test_precedence(debug);
	test_or(debug);
	test_and(debug);
	test_not(debug);
	test_greater(debug);
	test_lesser(debug);
	test_geq(debug);
	test_leq(debug);
	test_eq(debug);
	test_neq(debug);
	test_chained_cmp(debug);
	test_bitwise_and(debug);
	test_bitwise_or(debug);
	test_bitwise_leftshift(debug);
	test_bitwise_rightshift(debug);
	test_bitwise_xor(debug);
	test_bitwise_neg(debug);
	
	//from test_other.h
	status_test_transpiler("other");
	test_statuscode(debug);
	test_simplevar(debug);
	test_recursive(debug);
	test_charconst_cmp(debug);
	test_wrapped_expr(debug);
	
	//from test_stmt.h
	status_test_transpiler("stmts");
	test_ifstmt(debug);
	test_whilestmt(debug);
	test_subrcall(debug);
	test_break(debug);
	test_continue(debug);

	status_test_transpiler("Typeinference");
	test_suite_typeinference();

	status_test_transpiler("Typechecker");
    test_suite_typechecker();

    status_test_transpiler("PASSED ALL TESTS\n");
	
	return 0;
}
