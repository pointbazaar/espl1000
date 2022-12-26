#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_gen_tac.h"

static void case_no_index();
static void case_1_index();
static void case_2_index();
//static void case_n_index();

void test_gen_tac_simplevar(){

	case_no_index();
	if(0)case_1_index(); //TODO: re-enable this testcase
	if(0)case_2_index(); //TODO: re-enable this testcase
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
	
	vmcu_system_step_n(system, 20);
	
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

		//vmcu_system_step_n(system, 30);
		for(int i=0; i<30; i++){
			vmcu_system_step(system);

			uint8_t r1  = vmcu_system_read_gpr(system, 1);
			uint8_t r18 = vmcu_system_read_gpr(system, 18);
			uint8_t r19 = vmcu_system_read_gpr(system, 19);

			printf("r0=%d, r18=%d, r19=%d\n", r1,r18,r19);
		}

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		printf("expected:%d, actual=%d\n", value, r0);

		assert(r0 == value);

		vmcu_system_dtor(system);
	}
}

static void case_2_index(){
	
	status_test_codegen_tac("SimpleVar - 2 index");
	
	const uint16_t addr  = 0xc7;
	
	const int8_t value = rand()%0xff;
	
	const int index1 = (rand()%6)&0b01111111;
	const int index2 = (rand()%6)&0b01111111;
	
	char snippet[200];
	char* template = "fn main() -> int { %s }";
	char snippet2[200];
	char* body = "[[int]] arr = 0xc7; return arr[%d][%d];";
	sprintf(snippet2, body, index1, index2);
	sprintf(snippet, template, snippet2);
	
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//prepare a value in the location
	vmcu_system_write_data(system, addr+index1, 0xd1);
	
	vmcu_system_write_data(system, 0xd1+index2, value);
	
	vmcu_system_step_n(system, 30);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	assert(r0 == value);
	
	vmcu_system_dtor(system);
}
