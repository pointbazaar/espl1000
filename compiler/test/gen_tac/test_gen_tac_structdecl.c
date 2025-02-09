#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

void test_gen_tac_structdecl_case_read_struct() {

	status_test_codegen_tac("StructDecl - read struct");

	const uint8_t value = 0xc7;

	char snippet[200];
	char* template = "struct A{int8 c; int8 a; int8 b;} fn main() -> int { local A x; x.b = %d; return x.b; }";
	sprintf(snippet, template, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	for (int i = 0; i < 80; i++) {
		vmcu_system_step(system);
	}

	uint8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == value);

	vmcu_system_dtor(system);
}

void test_gen_tac_structdecl_case_write_struct() {

	status_test_codegen_tac("StructDecl - write struct");

	const uint8_t value = 0xc4;

	char snippet[200];
	char* template = "struct A{int8 a; int8 b;} fn main() -> int { local A x; x.b = %d; return x.b; }";
	sprintf(snippet, template, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 50);

	uint8_t actual = vmcu_system_read_gpr(system, 0);

	assert(actual == value);

	vmcu_system_dtor(system);
}
