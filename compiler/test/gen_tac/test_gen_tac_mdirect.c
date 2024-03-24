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

	for(uint16_t address = 0x100; address < 0x110; address++){
		const int8_t value = 0x34;

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
}

static void case_variable_addr(){

	status_test_codegen_tac("MDirect - variable Address");

	//we need an address here which is small enough
	//to fit into a register. 0xc7 is usable.
	for(uint16_t address = 0xc7; address < 0xd7; address++){

		const int8_t value = 0x38;

		char snippet[200];
		sprintf(snippet, "fn main() -> int { int y = 3; int x = [y+%d]; return x; }", address);

		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//prepare a value in the location
		vmcu_system_write_data(system, address+3, value);

		//vmcu_system_step_n(system, 30);

		for(int i=0; i < 40; i++){

			vmcu_system_step(system);

			//int8_t r0 = vmcu_system_read_gpr(system, 0);
			//int8_t r21= vmcu_system_read_gpr(system, 21);
			//int8_t r22= vmcu_system_read_gpr(system, 22);

			//printf("r0:0x%x, r21:r22:0x%x\n", r0, (r22 << 8) | r21);
		}

		//return value in r0
		int8_t actual = vmcu_system_read_gpr(system, 0);

		assert(value == actual);

		vmcu_system_dtor(system);
	}
}
