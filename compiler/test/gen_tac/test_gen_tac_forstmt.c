#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "test_gen_tac.h"

void test_gen_tac_forstmt_0_rounds() {

	status_test_codegen_tac("ForStmt 0 rounds");

	char snippet[200];
	const char* template = "fn main() -> int { int x = 0; for i in 1 .. 0 { x = x + 1; } return x; }";
	strcpy(snippet, template);

	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	vmcu_system_step_n(system, 22);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 0);

	vmcu_system_dtor(system);
}

void test_gen_tac_forstmt_1_rounds() {

	status_test_codegen_tac("ForStmt 1 rounds");

	const int8_t initial = 0x7;

	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; for i in 0 .. 0 { x = x + 1; } return x; }";
	sprintf(snippet, template, initial);

	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	vmcu_system_step_n(system, 100);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == initial + 1);

	vmcu_system_dtor(system);
}

void test_gen_tac_forstmt_1_rounds_break() {

	status_test_codegen_tac("ForStmt 1 rounds break");

	const int8_t initial = 0x8;

	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; for i in 0 .. 0 { break; x = x + 1; } return x; }";
	sprintf(snippet, template, initial);

	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	vmcu_system_step_n(system, 100);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == initial);

	vmcu_system_dtor(system);
}

void test_gen_tac_forstmt_n_rounds() {

	status_test_codegen_tac("ForStmt n rounds");

	const int8_t initial = 17;
	const int8_t bound = 4;

	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; for i in 1 .. %d { x = x + 1; } return x; }";
	sprintf(snippet, template, initial, bound);

	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	vmcu_system_step_n(system, 150);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == initial + bound);

	vmcu_system_dtor(system);
}
