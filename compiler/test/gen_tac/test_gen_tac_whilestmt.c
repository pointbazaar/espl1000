#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

void test_gen_tac_whilestmt_case_0_rounds() {

	status_test_codegen_tac("WhileStmt 0 rounds");

	const int8_t initial = 0;
	const int8_t bound = initial;

	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; while x < %d { x = x + 1; } return x; }";
	sprintf(snippet, template, initial, bound);

	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	vmcu_system_step_n(system, 20);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == initial);

	vmcu_system_dtor(system);
}

void test_gen_tac_whilestmt_case_1_rounds() {

	status_test_codegen_tac("WhileStmt 1 rounds");

	for (int8_t initial = 0; initial < 10; initial++) {
		const int8_t bound = initial + 1;

		char snippet[200];
		const char* template = "fn main() -> int { int x = %d; while x < %d { x = x + 1; } return x; }";
		sprintf(snippet, template, initial, bound);

		//compile snippet and init a vmcu
		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//step it past the main function
		vmcu_system_step_n(system, 80);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == bound);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_whilestmt_case_n_rounds() {

	status_test_codegen_tac("WhileStmt n rounds");

	for (int8_t initial = 0; initial < 3; initial++) {
		for (int8_t bound = initial; bound < initial + 4; bound++) {

			char snippet[200];
			const char* template = "fn main() -> int { int x = %d; while x < %d { x = x + 1; } return x; }";
			sprintf(snippet, template, initial, bound);

			//compile snippet and init a vmcu
			vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

			//step it past the main function
			vmcu_system_step_n(system, 100 + (bound - initial) * 20);

			int8_t r0 = vmcu_system_read_gpr(system, 0);

			assert(r0 == bound);

			vmcu_system_dtor(system);
		}
	}
}

void test_gen_tac_whilestmt_case_1_rounds_break() {

	status_test_codegen_tac("WhileStmt 1 rounds break");

	for (int8_t initial = 0; initial < 10; initial++) {
		const int8_t bound = initial + 1;

		char snippet[200];
		const char* template = "fn main() -> int { int x = %d; while x < %d { break; x = x + 1; } return x; }";
		sprintf(snippet, template, initial, bound);

		//compile snippet and init a vmcu
		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//step it past the main function
		vmcu_system_step_n(system, 50);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == initial);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_whilestmt_case_1_rounds_continue() {

	status_test_codegen_tac("WhileStmt 1 rounds continue");

	for (int8_t initial = 0; initial < 10; initial++) {

		char snippet[200];
		const char* template = "fn main() -> int8 { int8 x=0; int8 y=%d; while x < 1 { x=x+1; continue; y=y+1; } return y; }";
		sprintf(snippet, template, initial);

		//compile snippet and init a vmcu
		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//step it past the main function
		vmcu_system_step_n(system, 80);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == initial);

		vmcu_system_dtor(system);
	}
}
