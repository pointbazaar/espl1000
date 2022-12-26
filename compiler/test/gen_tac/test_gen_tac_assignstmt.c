#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

static void case_local_int();
static void case_local_struct();
static void case_local_array();

void test_gen_tac_assignstmt(){
		
	case_local_int();
	case_local_struct();
	case_local_array();
}

static void case_local_int(){
	
	status_test_codegen_tac("AssignStmt - local int");
	
	const int8_t value = rand()%0xff;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d; return x; }", value);
	
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 20);
	
	//assert that value is returned in r0 as it should be
	
	int8_t value1 = vmcu_system_read_gpr(system, 0);
		
	assert(value1 == value);
	
	vmcu_system_dtor(system);
}

static void case_local_struct(){
	
	status_test_codegen_tac("AssignStmt - local struct [TODO]");
	return;
	
	const int8_t value = rand()%0xff;
	
	char snippet[200];
	sprintf(snippet, "struct A {int x;}; fn main() -> int { A m; m.x = %d; return m.x; }", value);
	
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 20);
	
	//assert that value is returned in r0 as it should be
	
	int8_t value1 = vmcu_system_read_gpr(system, 0);
		
	assert(value1 == value);
	
	vmcu_system_dtor(system);
}

static void case_local_array(){

	status_test_codegen_tac("AssignStmt - local array");

	const int8_t value = rand()%0xff;

	char snippet[200];
	char* template = "fn main() -> int { [int] m = 0xc7; m[1] = %d; return m[1]; }";
	sprintf(snippet, template, value);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	vmcu_system_step_n(system, 54);

	int8_t value1 = vmcu_system_read_gpr(system, 0);

	int8_t value2 = vmcu_system_read_data(system, 0xc7+1);

	assert(value1 == value);
	assert(value2 == value);

	vmcu_system_dtor(system);
}
