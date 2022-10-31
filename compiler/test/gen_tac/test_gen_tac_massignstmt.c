#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

static void case_const_addr();
static void case_variable_addr();

void test_gen_tac_massignstmt(){
		
	case_const_addr();
	case_variable_addr();
}

static void case_const_addr(){
	
	status_test_codegen_tac("MAssignStmt - const Address");
	
	const int8_t value = rand()%0xff;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { [0x100] = %d; return 0; }", value);
	
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	int8_t actual = vmcu_system_read_data(system, 0x100);
	
	assert(actual == value);
	
	vmcu_system_dtor(system);
}

static void case_variable_addr(){
	
	status_test_codegen_tac("MAssignStmt - variable Address");
	
	const uint8_t addr   = 0xc7;
	const uint8_t offset = rand()%16;
	
	const int8_t value   = rand()%0xff;
	
	char snippet[200];
	char* template = "fn main() -> int { int x = %d; [x + 0xc7] = %d; return 0; }";
	sprintf(snippet, template, offset, value);
	
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	int8_t actual = vmcu_system_read_data(system, addr + offset);
	
	assert(actual == value);
	
	vmcu_system_dtor(system);
}
