#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

#include "transpiler.h"

#include "util/flags.h"

#include "test.h"
#include "util/test_statuscode.h"
#include "typeinference/test_typeinference.h"

#include "code_gen/test_op.h"
#include "code_gen/test_other.h"
#include "code_gen/test_assign.h"
#include "code_gen/test_stmt.h"

static void status(char* msg){
	printf("[TRANSPILER][TEST-SUITE] %s\n", msg);
}

int transpiler_test_all(bool debug){
	
	status("Running tests for smalldragon/transpiler:");
	
	//from test_assign.h
	status("transpiling of assignments");
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
	status("transpiling of operators");
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
	status("other");
	test_statuscode(debug);
	test_simplevar(debug);
	test_recursive(debug);
	test_charconst_cmp(debug);
	test_wrapped_expr(debug);
	
	//from test_stmt.h
	status("stmts");	
	test_ifstmt(debug);
	test_whilestmt(debug);
	test_subrcall(debug);
	test_break(debug);
	test_continue(debug);
	
	//from test_typeinference.h
	status("typeinference");
	test_infer_type_term(debug);
	test_infer_type_unopterm(debug);
	test_infer_type_expr(debug);
	test_infer_type_expr_multiple_terms(debug);
		
	status("PASSED ALL TESTS\n");
	
	return 0;
}
