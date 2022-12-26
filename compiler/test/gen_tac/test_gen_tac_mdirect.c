#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

static void case_const_addr();
static void case_variable_addr();

void test_gen_tac_mdirect(){
	
	case_const_addr();
	case_variable_addr();
}

static void case_const_addr(){
	
	status_test_codegen_tac("MDirect - const Address");
	
	const uint16_t address = 0x100+rand()%100;
	const int8_t value     = rand()%100;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = [%d]; return 0; }", address);
	
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//prepare a value in the location
	vmcu_system_write_data(system, address, value);
	
	vmcu_system_step_n(system, 10);
	
	//assert that the MDirect load has happened
	//and that the value is in a register or in the stack frame
	
	bool found = false;
	
	for(int i=0; i < 32; i++){
		int8_t value1 = vmcu_system_read_gpr(system, i);
		if(value1 == value) found = true;
	}
	
	assert(found);
	
	vmcu_system_dtor(system);
}

static void case_variable_addr(){

	status_test_codegen_tac("MDirect - variable Address");

	//we need an address here which is small enough
	//to fit into a register. 0xc7 is usable.
	const uint16_t address = 0xc7 + rand()%16;

	const int8_t value     = rand()%100;

	char snippet[200];
	sprintf(snippet, "fn main() -> int { int y = 3; int x = [y+%d]; return x; }", address);

	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//prepare a value in the location
	vmcu_system_write_data(system, address+3, value);

	vmcu_system_step_n(system, 30);

	//return value in r0
	int8_t actual = vmcu_system_read_gpr(system, 0);

	assert(value == actual);

	vmcu_system_dtor(system);
}
