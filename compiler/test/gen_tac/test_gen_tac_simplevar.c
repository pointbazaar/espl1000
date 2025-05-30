#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

void test_gen_tac_simplevar_case_no_index() {

	status_test_codegen_tac("SimpleVar - no index");

	const int8_t value = 0x34;

	char snippet[200];
	char* template = "fn main() -> int { int x = %d; return x; }";
	sprintf(snippet, template, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//prepare a value in the location
	//vmcu_system_write_data(system, address, value);

	vmcu_system_step_n(system, 20);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == value);

	vmcu_system_dtor(system);
}

void test_gen_tac_simplevar_case_1_index() {

	status_test_codegen_tac("SimpleVar - 1 index");

	const uint16_t addr = 0xc7;
	const int8_t value = 0x12;

	for (int index = 1; index < 4; index++) {

		char snippet[200];
		char* template = "fn main() -> int { [int] arr = 0xc7; return arr[%d]; }";
		sprintf(snippet, template, index);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//prepare a value in the location
		vmcu_system_write_data(system, addr + index, value);

		//printf("prepared addr: 0x%x\n", addr+index);
		//printf("prepared value: 0x%x\n", value);

		//vmcu_system_step_n(system, 30);
		for (int i = 0; i < 40; i++) {
			vmcu_system_step(system);

			//int8_t r0  = vmcu_system_read_gpr(system, 0);
			//int8_t r18 = vmcu_system_read_gpr(system, 18);
			//int8_t r19 = vmcu_system_read_gpr(system, 19);

			//printf("r0=%d, r19:r18=0x%x\n", r0, (r19 << 8) | r18);
		}

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		//printf("expected:0x%x, actual=0x%x\n", value, r0);

		assert(r0 == value);

		vmcu_system_dtor(system);
	}
}

void test_gen_tac_simplevar_case_2_index() {

	status_test_codegen_tac("SimpleVar - 2 index");

	const uint16_t addr = 0xc7;

	const int8_t value = 0x37;

	const int index1 = (3) & 0b01111111;
	const int index2 = (2) & 0b01111111;

	char snippet[400];
	char* template = "fn main() -> int { %s }";
	char snippet2[300];
	char* body = "[[int]] arr = 0xc7; return arr[%d][%d];";
	sprintf(snippet2, body, index1, index2);
	sprintf(snippet, template, snippet2);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//prepare a value in the location
	vmcu_system_write_data(system, addr + index1, 0xd1);

	vmcu_system_write_data(system, 0xd1 + index2, value);

	vmcu_system_step_n(system, 50);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == value);

	vmcu_system_dtor(system);
}
