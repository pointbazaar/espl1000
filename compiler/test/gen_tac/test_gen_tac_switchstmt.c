#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

static void test_gen_tac_switchstmt_first_case();
static void test_gen_tac_switchstmt_fallthrough_case();
static void test_gen_tac_switchstmt_no_case();

void test_gen_tac_switchstmt(){
		
	test_gen_tac_switchstmt_first_case();
	test_gen_tac_switchstmt_fallthrough_case();
	test_gen_tac_switchstmt_no_case();
}


static void test_gen_tac_switchstmt_first_case(){
	
	status_test_codegen_tac("SwitchStmt first case");
	
	int8_t value = rand()%0xff;
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = 0; switch x { case 0 { return %d; }} return 0; }";
	sprintf(snippet, template, value);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	vmcu_system_step_n(system, 40);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
		
	assert(r0 == value);
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_switchstmt_fallthrough_case(){
	
	status_test_codegen_tac("SwitchStmt fallthrough case");
	
	int8_t value = rand()%0xff;
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = 0; switch x { case 0 case 1 { return %d; }} return 0; }";
	sprintf(snippet, template, value);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	vmcu_system_step_n(system, 40);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
		
	assert(r0 == value);
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_switchstmt_no_case(){
	
	status_test_codegen_tac("SwitchStmt no case");
	
	int8_t value = 1+rand()%0xfe;
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = 3; switch x { case 0 case 1 { return 0; }} return %d; }";
	sprintf(snippet, template, value);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	vmcu_system_step_n(system, 40);
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
		
	assert(r0 == value);
	
	vmcu_system_dtor(system);
}
