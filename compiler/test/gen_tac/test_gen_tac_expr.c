#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

//test each operator with 2 random values
static void test_gen_tac_expr_plus();
static void test_gen_tac_expr_minus();
static void test_gen_tac_expr_mul();
static void test_gen_tac_expr_and();
static void test_gen_tac_expr_or();
static void test_gen_tac_expr_shift_left();
static void test_gen_tac_expr_shift_right();
static void test_gen_tac_expr_xor();

void test_gen_tac_expr(){
	
	test_gen_tac_expr_plus();
	test_gen_tac_expr_minus();
	test_gen_tac_expr_mul();
	test_gen_tac_expr_and();
	test_gen_tac_expr_or();
	test_gen_tac_expr_shift_left();
	test_gen_tac_expr_shift_right();
	test_gen_tac_expr_xor();
}


static void test_gen_tac_expr_plus(){

	status_test_codegen_tac("Expr +");

	for(int8_t value1 = 0; value1 < 4; value1++){
		for(int8_t value2 = 0; value2 < 4; value2++){

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d + %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 30);

			//assert that value is returned in r0 as it should be

			int8_t r0_value = vmcu_system_read_gpr(system, 0);

			assert(r0_value == (value1 + value2));

			vmcu_system_dtor(system);
		}
	}
}

static void test_gen_tac_expr_minus(){

	status_test_codegen_tac("Expr -");

	for(int8_t value1 = 50; value1 < 55; value1++){
		for(int8_t value2 = 0; value2 < 10; value2++){
			const int8_t expected = value1 - value2;

			char snippet[200];
			sprintf(snippet, "fn main() -> int8 { int8 x = %d - %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 30);

			//assert that value is returned in r0 as it should be
			int8_t r0 = vmcu_system_read_gpr(system, 0);

			assert(r0 == expected);

			vmcu_system_dtor(system);
		}
	}
}

static void test_gen_tac_expr_mul(){

	return;

	status_test_codegen_tac("Expr *");

	for(int8_t value1 = 0; value1 < 5; value1++){
		for(int8_t value2 = 0; value2 < 5; value2++){

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d * %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 20);

			//assert that value is returned in r0 as it should be
			int8_t r0_value = vmcu_system_read_gpr(system, 0);

			assert(r0_value == (value1 * value2));

			vmcu_system_dtor(system);
		}
	}
}

static void test_gen_tac_expr_and(){

	status_test_codegen_tac("Expr &");

	for(int8_t value1 = 0; value1 < 10; value1++){
		for(int8_t value2 = 0; value2 < 10; value2++){

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d & %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 30);

			//assert that value is returned in r0 as it should be

			int8_t r0_value = vmcu_system_read_gpr(system, 0);

			assert(r0_value == (value1 & value2));

			vmcu_system_dtor(system);
		}
	}
}

static void test_gen_tac_expr_or(){

	status_test_codegen_tac("Expr |");

	for(int8_t value1 = 0; value1 < 10; value1++){
		for(int8_t value2 = 0; value2 < 10; value2++){

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d | %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			int8_t r0_value = vmcu_system_read_gpr(system, 0);

			assert(r0_value == (value1 | value2));

			vmcu_system_dtor(system);
		}
	}
}


static void test_gen_tac_expr_shift_left(){

	status_test_codegen_tac("Expr <<");

	for(int8_t value1 = 0; value1 < 10; value1++){
		for(int8_t value2 = 1; value2 < 5; value2++){

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d << %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const int8_t r0_value = vmcu_system_read_gpr(system, 0);
			const int8_t expect   = value1 << value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

static void test_gen_tac_expr_shift_right(){

	status_test_codegen_tac("Expr >>");

	for(int8_t value1 = 0; value1 < 10; value1++){
		for(int8_t value2 = 1; value2 < 5; value2++){

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d >> %d; return x; }", value1, value2);

			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			vmcu_system_step_n(system, 28);

			const int8_t r0_value = vmcu_system_read_gpr(system, 0);
			const int8_t expect   = value1 >> value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}

static void test_gen_tac_expr_xor(){

	status_test_codegen_tac("Expr ^");

	for(int8_t value1 = 0; value1 < 10; value1++){
		for(int8_t value2 = 0; value2 < 10; value2++){

			char snippet[200];
			sprintf(snippet, "fn main() -> int { int x = %d ^ %d; return x; }", value1, value2);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 28);

			//assert that value is returned in r0 as it should be

			const int8_t r0_value = vmcu_system_read_gpr(system, 0);
			const int8_t expect   = value1 ^ value2;

			assert(r0_value == expect);

			vmcu_system_dtor(system);
		}
	}
}
