#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

static void case_no_index();
static void case_1_index();
//static void case_2_index();
//static void case_n_index();

void test_gen_tac_simplevar(){
	
	case_no_index();
	case_1_index();
}

static void case_no_index(){
	
	status_test_codegen_tac("SimpleVar - no index");
	
	const int8_t value = rand()%0xff;
	
	char snippet[200];
	char* template = "fn main() -> int { int x = %d; return x; }";
	sprintf(snippet, template, value);
	
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//prepare a value in the location
	//vmcu_system_write_data(system, address, value);
	
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == value);
	
	vmcu_system_dtor(system);
}

static void case_1_index(){
	
	status_test_codegen_tac("SimpleVar - 1 index");
	
	const uint16_t addr  = 0xc7;
	const int8_t value = rand()%0xff;
	
	
	for(int index = 1; index < 4; index++){
		//const int index = 1;
		
		char snippet[200];
		char* template = "fn main() -> int { [int] arr = 0xc7; return arr[%d]; }";
		sprintf(snippet, template, index);
		
		vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

		//prepare a value in the location
		vmcu_system_write_data(system, addr+index, value);
		
		for(int i=0; i < 30; i++){
			vmcu_system_step(system);
		}
		
		int8_t r0 = vmcu_system_read_gpr(system, 0);
		
		assert(r0 == value);
		
		vmcu_system_dtor(system);
	}
}
