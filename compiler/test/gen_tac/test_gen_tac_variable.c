#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

void test_gen_tac_variable_case_no_member_access() {

	status_test_codegen_tac("Variable - no member access");

	for (int8_t value = 0; value < 10; value++) {

		char snippet[200];
		char* template = "fn main() -> int { int x = %d; return x; }";
		sprintf(snippet, template, value);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == value);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_variable_case_1_member_access() {

	status_test_codegen_tac("Variable - 1 member access");

	for (int8_t value = 0; value < 10; value++) {

		char snippet[200];
		char* template = "struct A {int8 a; int8 b;}fn main() -> int { A m = 0xc7; return m.b; }";
		sprintf(snippet, template, value);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//because int8 a is before int8 b
		const int member_offset = 1;

		vmcu_system_write_data(system, 0xc7 + member_offset, value);

		vmcu_system_step_n(system, 40);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		if (r0 != value) {
			printf("r0 == 0x%x, expected 0x%x\n", r0, value);
		}

		assert(r0 == value);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_variable_case_2_member_access() {

	status_test_codegen_tac("Variable - 2 member access");

	for (int8_t value = 0; value < 10; value++) {

		char snippet[200];
		char* template = "struct A { A a; int8 b;}fn main() -> int { A m = 0xc7; return m.a.b; }";
		sprintf(snippet, template, value);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		const int member_offset = 2;

		//set m.a
		vmcu_system_write_data(system, 0xc7, 0xd7);

		vmcu_system_write_data(system, 0xd7 + member_offset, value);

		vmcu_system_step_n(system, 50);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == value);

		vmcu_system_dtor(system);
	}
}
