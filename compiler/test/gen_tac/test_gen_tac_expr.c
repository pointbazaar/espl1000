#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_gen_tac.h"

//test each operator with 2 random values
static void test_gen_tac_expr_plus();
static void test_gen_tac_expr_minus();
static void test_gen_tac_expr_mul();
static void test_gen_tac_expr_and();
static void test_gen_tac_expr_or();
static void test_gen_tac_expr_shift_left();
static void test_gen_tac_expr_shift_right();
static void test_gen_tac_expr_xor();

void test_gen_tac_expr(){
	
	test_gen_tac_expr_plus();
	test_gen_tac_expr_minus();
	test_gen_tac_expr_mul();
	test_gen_tac_expr_and();
	test_gen_tac_expr_or();
	test_gen_tac_expr_shift_left();
	test_gen_tac_expr_shift_right();
	test_gen_tac_expr_xor();
}


static void test_gen_tac_expr_plus(){
	
	status_test_codegen_tac("Expr +");
	
	const int8_t value1 = rand()%50;
	const int8_t value2 = rand()%50;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d + %d; return x; }", value1, value2);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	int8_t r0_value = vmcu_system_read_gpr(system, 0);
		
	assert(r0_value == (value1 + value2));
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_expr_minus(){
	
	status_test_codegen_tac("Expr -");
	
	const int8_t value1 = 50+rand()%50;
	const int8_t value2 = rand()%50;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d - %d; return x; }", value1, value2);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	int8_t r0_value = vmcu_system_read_gpr(system, 0);
		
	assert(r0_value == (value1 - value2));
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_expr_mul(){
	
	return;
	
	status_test_codegen_tac("Expr *");
	
	const int8_t value1 = rand()%10;
	const int8_t value2 = rand()%10;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d * %d; return x; }", value1, value2);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	int8_t r0_value = vmcu_system_read_gpr(system, 0);
		
	assert(r0_value == (value1 * value2));
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_expr_and(){

	status_test_codegen_tac("Expr &");
	
	const int8_t value1 = rand()%0xff;
	const int8_t value2 = rand()%0xff;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d & %d; return x; }", value1, value2);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 20; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	int8_t r0_value = vmcu_system_read_gpr(system, 0);
		
	assert(r0_value == (value1 & value2));
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_expr_or(){

	status_test_codegen_tac("Expr |");
	
	const int8_t value1 = rand()%0xff;
	const int8_t value2 = rand()%0xff;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d | %d; return x; }", value1, value2);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 18; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	int8_t r0_value = vmcu_system_read_gpr(system, 0);
		
	assert(r0_value == (value1 | value2));
	
	vmcu_system_dtor(system);
}


static void test_gen_tac_expr_shift_left(){
	
	status_test_codegen_tac("Expr <<");
	
	const int8_t value1 = rand()%0xff;
	const int8_t value2 = 1+rand()%4;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d << %d; return x; }", value1, value2);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 18; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	const int8_t r0_value = vmcu_system_read_gpr(system, 0);
	const int8_t expect   = value1 << value2;
	
	assert(r0_value == expect);
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_expr_shift_right(){
	
	status_test_codegen_tac("Expr >>");
	
	const int8_t value1 = rand()%0xff;
	const int8_t value2 = 1+rand()%4;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d >> %d; return x; }", value1, value2);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 18; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	const int8_t r0_value = vmcu_system_read_gpr(system, 0);
	const int8_t expect   = value1 >> value2;
		
	assert(r0_value == expect);
	
	vmcu_system_dtor(system);
}

static void test_gen_tac_expr_xor(){

	status_test_codegen_tac("Expr ^");
	
	const int8_t value1 = rand()%0xff;
	const int8_t value2 = rand()%0xff;
	
	char snippet[200];
	sprintf(snippet, "fn main() -> int { int x = %d ^ %d; return x; }", value1, value2);
	
	//compile snippet and init a vmcu
	vmcu_system_t* system = prepare_vmcu_system_from_code_snippet(snippet);

	//step it past the main function
	for(int i=0; i < 18; i++){
		vmcu_system_step(system);
	}
	
	//assert that value is returned in r0 as it should be
	
	const int8_t r0_value = vmcu_system_read_gpr(system, 0);
	const int8_t expect   = value1 ^ value2;
		
	assert(r0_value == expect);
	
	vmcu_system_dtor(system);
}
