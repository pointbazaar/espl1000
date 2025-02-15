#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

void test_gen_tac_call_case_no_args() {

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

void test_gen_tac_call_case_1_args_return() {

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

void test_gen_tac_call_case_frame_pointer_restored_after_call() {

	status_test_codegen_tac("Call - frame pointer YL:YH restored after call (caller has no locals)");

	char* template = "fn main()->int{ return f(3); }"
			 " fn f(uint8 a)->int{ "
			 "uint8 b = 83; "
			 "uint8 c = 3; "
			 "uint8 d = 4; "
			 "return b; "
			 "}";

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(template);

	vmcu_system_step_n(system, 50);

	const uint8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 83);

	uint16_t sp = vmcu_system_read_sp(system);
	uint16_t y = vmcu_system_read_y(system);

	if (sp != y + 2) {
		printf("sp=0x%x, Y=0x%x\n", sp, y);
	}

	// This is because 'main' was not called,
	// it is the first function to run after the initial boilerplate.
	// So since we did return, SP will be higher by 2
	assert(sp == y + 2);

	vmcu_system_dtor(system);
}

void test_gen_tac_call_case_frame_pointer_restored_after_call_caller_has_locals() {

	status_test_codegen_tac("Call - frame pointer YL:YH restored after call (caller has locals)");

	char* template = "fn main()->int{ "
			 "uint8 x = 3; "
			 "return f(3); "
			 "} "
			 "fn f(uint8 a)->int{ "
			 "uint8 b = 73; "
			 "return b; "
			 "} ";

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(template);

	vmcu_system_step_n(system, 50);

	const uint8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 73);

	uint16_t sp = vmcu_system_read_sp(system);
	uint16_t y = vmcu_system_read_y(system);

	if (sp != y + 3) {
		printf("sp=0x%x, Y=0x%x\n", sp, y);
	}

	// This is because 'main' was not called,
	// it is the first function to run after the initial boilerplate.
	// So since we did return, SP will be higher by 2.
	// And since the frame pointer YL:YH is created after space has been made on the stack
	// for local variables
	// (our frame pointer only ever applies offsets >= 0 for both locals and args),
	// YL:YH will further be higher by 1 byte due to the uint8 local variable.
	assert(sp == y + 3);

	vmcu_system_dtor(system);
}

void test_gen_tac_call_case_1_args_write_8bit() {

	status_test_codegen_tac("Call - 1 args - write to SRAM (8 bit)");

	const uint8_t expected = 0xab;

	char snippet[200];
	char* template = "fn main()->int{ f(%d); return 0; } fn f(uint8 a)->int { [0x100, 1]=a; return 0; }";
	sprintf(snippet, template, expected);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 50);

	const uint8_t actual = vmcu_system_read_data(system, 0x100);

	assert(actual == expected);

	vmcu_system_dtor(system);
}

void test_gen_tac_call_case_1_args_write_16bit() {

	status_test_codegen_tac("Call - 1 args - write to SRAM (16 bit)");

	const uint16_t expected = 0x5973;

	char snippet[200];
	char* template = "fn main()->int{ f(%d); return 0; } fn f(uint16 a)->int { [0x100, 2]=a; return 0; }";
	sprintf(snippet, template, expected);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 50);

	const uint16_t actual = vmcu_system_read_data16(system, 0x100);

	if (actual != expected) {
		printf("actual: 0x%x\n", actual);
		printf("expected: 0x%x\n", expected);
	}

	assert(actual == expected);

	vmcu_system_dtor(system);
}

void test_gen_tac_call_case_1_args_write_3fns() {

	status_test_codegen_tac("Call - 1 args - write to SRAM - 3 functions");

	const uint8_t expected = (0x20) | 0x1;

	char snippet[200];
	char* template = "fn main()->int{ return f1(%d); } fn f1(uint8 a)->int {return f2(a);} fn f2 (uint8 b)->int{ [0x101, 1]=b; return 0; }";
	sprintf(snippet, template, expected);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 50);

	const uint8_t actual = vmcu_system_read_data(system, 0x101);

	//printf("expected=0x%x, actual=0x%x\n", expected, actual);

	assert(actual == expected);

	vmcu_system_dtor(system);
}

void test_gen_tac_call_case_2_args() {

	status_test_codegen_tac("Call - 2 args");

	const int8_t value = 0xab;

	char snippet[200];
	char* template = "fn main()->int{int x = f(3,2); return x;} fn f(int a, int b)->int{return %d+a+b;}";
	sprintf(snippet, template, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 80);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == (int8_t)(value + 3 + 2));

	//check if SP is same as before main
	uint16_t sp = vmcu_system_read_sp(system);
	assert(sp == 0x085f);

	vmcu_system_dtor(system);
}
