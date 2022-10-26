#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

static void test_gen_tac_forstmt_0_rounds();
static void test_gen_tac_forstmt_1_rounds();
static void test_gen_tac_forstmt_n_rounds();

void test_gen_tac_forstmt(){
		
	test_gen_tac_forstmt_0_rounds();
	test_gen_tac_forstmt_1_rounds();
	test_gen_tac_forstmt_n_rounds();
}


static void test_gen_tac_forstmt_0_rounds(){
	
	status_test_codegen_tac("ForStmt 0 rounds");
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = 0; for i in 1 .. 0 { x = x + 1; } return x; }";
	sprintf(snippet, template);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
		
	assert(r0 == 0);
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_forstmt_1_rounds(){
	
	status_test_codegen_tac("ForStmt 1 rounds");
	
	const int8_t initial = rand()%10;
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; for i in 0 .. 0 { x = x + 1; } return x; }";
	sprintf(snippet, template, initial);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 100; i++){
		vmcu_system_step(system);
	}	
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
		
	assert(r0 == initial + 1);
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_forstmt_n_rounds(){
	
	status_test_codegen_tac("ForStmt n rounds");
	
	const int8_t initial = rand()%30;
	const int8_t bound = 1+rand()%7;
	
	char snippet[200];
	const char* template = "fn main() -> int { int x = %d; for i in 1 .. %d { x = x + 1; } return x; }";
	sprintf(snippet, template, initial, bound);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 150; i++){
		vmcu_system_step(system);
	}
	
	int8_t r0 = vmcu_system_read_gpr(system, 0);
		
	assert(r0 == initial + bound);
	
	vmcu_system_dtor(system);
}
