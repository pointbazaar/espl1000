#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

//local struct in stack frame
//static void case_read_local_struct();
//static void case_write_local_struct();

//structs anywhere, referenced by pointer
static void case_read_struct();
static void case_write_struct();

void test_gen_tac_structdecl() {

	case_read_struct();
	case_write_struct();
}

static void case_read_struct() {

	status_test_codegen_tac("StructDecl - read struct");

	const int8_t value = 0xc7;

	char snippet[200];
	char* template = "struct A{int8 a; int8 b;} fn main() -> int { A x = 0x80; return x.b; }";
	sprintf(snippet, template);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//prepare a value in the location
	vmcu_system_write_data(system, 0x80 + 1, value);

	vmcu_system_step_n(system, 50);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == value);

	vmcu_system_dtor(system);
}

static void case_write_struct() {

	status_test_codegen_tac("StructDecl - write struct");

	const int8_t value = 0xc4;

	char snippet[200];
	char* template = "struct A{int8 a; int8 b;} fn main() -> int { A x = 0x80; x.b = %d; return 0; }";
	sprintf(snippet, template, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 50);

	int8_t actual = vmcu_system_read_data(system, 0x80 + 1);

	//printf("expected:0x%x, actual:0x%x\n", (uint8_t)value, (uint8_t)actual);

	assert(actual == value);

	vmcu_system_dtor(system);
}
