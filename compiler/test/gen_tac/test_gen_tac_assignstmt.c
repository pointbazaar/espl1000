#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

static void case_local_int();
static void case_local_struct();

void test_gen_tac_assignstmt(){
		
	case_local_int();
	case_local_struct();
}


static void case_local_int(){
	
	status_test_codegen_tac("AssignStmt - local int");
	
	const int8_t value = rand()%0xff;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d; return x; }", value);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
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
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	int8_t value1 = vmcu_system_read_gpr(system, 0);
		
	assert(value1 == value);
	
	vmcu_system_dtor(system);
}
