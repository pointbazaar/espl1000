#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

static void case_no_args();
static void case_1_args_return();
static void case_1_args_write();
static void case_1_args_write_3fns();
static void case_2_args();

void test_gen_tac_call(){

	case_no_args();
	case_1_args_return();
	case_1_args_write();
	case_1_args_write_3fns();
	case_2_args();
}

static void case_no_args(){

	status_test_codegen_tac("Call - no args");

	const int8_t value = 0x33;

	char snippet[200];
	char* template = "fn main()->int{int x = f(); return x;} fn f()->int{return %d;}";
	sprintf(snippet, template, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 45);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == value);

	//check if SP is same as before main
	uint16_t sp = vmcu_system_read_sp(system);
	assert(sp == 0x085f);

	vmcu_system_dtor(system);
}

static void case_1_args_return(){

	status_test_codegen_tac("Call - 1 args - return value");

	const int8_t value = 33;

	char snippet[200];
	char* template = "fn main()->int{int x = f(3); return x;} fn f(int a)->int{return %d+a;}";
	sprintf(snippet, template, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 50);

	const int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 36);

	//check if SP is same as before main
	uint16_t sp = vmcu_system_read_sp(system);
	assert(sp == 0x085f);

	vmcu_system_dtor(system);
}

static void case_1_args_write(){

	status_test_codegen_tac("Call - 1 args - write to SRAM");

	const uint8_t expected = 0xab;

	char snippet[200];
	char* template = "fn main()->int{ f(%d); return 0; } fn f(uint8 a)->int { [0x100]=a; return 0; }";
	sprintf(snippet, template, expected);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 50);

	const uint8_t actual = vmcu_system_read_data(system, 0x100);

	assert(actual == expected);

	vmcu_system_dtor(system);
}

static void case_1_args_write_3fns(){

	status_test_codegen_tac("Call - 1 args - write to SRAM - 3 functions");

	const uint8_t expected = (0x20) | 0x1;

	char snippet[200];
	char* template = "fn main()->int{ return f1(%d); } fn f1(uint8 a)->int {return f2(a);} fn f2 (uint8 b)->int{ [0x101]=b; return 0; }";
	sprintf(snippet, template, expected);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 50);

	const uint8_t actual = vmcu_system_read_data(system, 0x101);

	//printf("expected=0x%x, actual=0x%x\n", expected, actual);

	assert(actual == expected);

	vmcu_system_dtor(system);
}

static void case_2_args(){

	status_test_codegen_tac("Call - 2 args");

	const int8_t value = 0xab;

	char snippet[200];
	char* template = "fn main()->int{int x = f(3,2); return x;} fn f(int a, int b)->int{return %d+a+b;}";
	sprintf(snippet, template, value);
	
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);
	
	vmcu_system_step_n(system, 80);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == (int8_t)(value+3+2));
	
	//check if SP is same as before main
	uint16_t sp = vmcu_system_read_sp(system);
    assert(sp == 0x085f);
	
	vmcu_system_dtor(system);
}
