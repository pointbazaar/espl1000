#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "test_x86_code_gen.h"
#include "test_x86_code_gen_util.h"

#include "compile_ir/test_compile_tac.h"

void status_test_x86_codegen(char* msg) {
	printf(" - [TEST] x86 codegen %s\n", msg);
}

void test_suite_x86_code_gen() {

	test_x86_compile_tac_nop();
	test_x86_compile_tac_const_value();

	test_x86_compile_tac_store_const_addr();
	//test_x86_compile_tac_load_const_addr();

	//test_x86_compile_tac_binary_op_immediate();
	//test_x86_compile_tac_unary_op();
	//test_x86_compile_tac_binary_op();

	test_x86_compile_tac_goto();
	//test_x86_compile_tac_if_goto();
	//test_x86_compile_tac_if_cmp_goto();

	//test_x86_compile_tac_return();
	test_x86_compile_tac_copy();
	//test_x86_compile_tac_param();
	//test_x86_compile_tac_call();
	//test_x86_compile_tac_setup_stackframe();

	//test_x86_compile_tac_load();
	//test_x86_compile_tac_store();
}
