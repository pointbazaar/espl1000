#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

static void case_no_args();
static void case_1_args();
static void case_2_args();

void test_gen_tac_call(){
	
	case_no_args();
	case_1_args();
	case_2_args();
}

static void case_no_args(){
	
	status_test_codegen_tac("Call - no args");
	
	const int8_t value = rand()%0xff;
	
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

static void case_1_args(){

	status_test_codegen_tac("Call - 1 args");

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


static void case_2_args(){
	
	status_test_codegen_tac("Call - 2 args");
	
	const int8_t value = rand()%0xff;
	
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
