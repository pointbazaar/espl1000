#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

void test_gen_tac_mdirect(){
		
	status_test_codegen_tac("MDirect");
	
	const uint16_t address = 0x40+rand()%100;
	const int8_t value     = rand()%0xff;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = [%d]; return 0; }", address);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//prepare a value in the location
	vmcu_system_write_data(system, address, value);

	//step it a few times
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
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
