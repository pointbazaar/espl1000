#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

static void test_gen_tac_whilestmt_0_rounds();
static void test_gen_tac_whilestmt_1_rounds();
static void test_gen_tac_whilestmt_n_rounds();

void test_gen_tac_whilestmt(){
		
	test_gen_tac_whilestmt_0_rounds();
	test_gen_tac_whilestmt_1_rounds();
	test_gen_tac_whilestmt_n_rounds();
}


static void test_gen_tac_whilestmt_0_rounds(){
	
	status_test_codegen_tac("WhileStmt 0 rounds");
	
	const int8_t initial = 0;
	const int8_t bound = initial;
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; while x < %d { x = x + 1; } return x; }";
	sprintf(snippet, template, initial, bound);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
		
	assert(r0 == initial);
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_whilestmt_1_rounds(){
	
	status_test_codegen_tac("WhileStmt 1 rounds");
	
	const int8_t initial = rand()%10;
	const int8_t bound = initial+1;
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; while x < %d { x = x + 1; } return x; }";
	sprintf(snippet, template, initial, bound);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 50; i++){
		vmcu_system_step(system);
	}	
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
		
	assert(r0 == bound);
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_whilestmt_n_rounds(){
	
	status_test_codegen_tac("WhileStmt n rounds");
	
	const int8_t initial = rand()%10;
	const int8_t bound = initial+(rand()%10);
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; while x < %d { x = x + 1; } return x; }";
	sprintf(snippet, template, initial, bound);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 100; i++){
		vmcu_system_step(system);
	}
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
	
	printf("expected: %d, actual: %d", bound, r0);
	fflush(stdout);
		
	assert(r0 == bound);
	
	vmcu_system_dtor(system);
}
