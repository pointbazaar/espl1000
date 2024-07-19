#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "typechecker/test_typechecker.h"
#include "typeinference/test_typeinference.h"
#include "avr_code_gen/test_avr_code_gen.h"
#include "gen_tac/test_gen_tac.h"

static void status_test_transpiler(char* msg) {
	printf("[Compiler][TEST-SUITE] %s\n", msg);
}

int main() {

	status_test_transpiler("Running tests for smalldragon/compiler:");

	status_test_transpiler("Typeinference");
	test_suite_typeinference();

	status_test_transpiler("Typechecker");
	test_suite_typechecker();

	status_test_transpiler("AVR CodeGen");
	test_suite_avr_code_gen();

	status_test_transpiler("TAC CodeGen from Snippets");
	test_suite_tac_codegen();

	status_test_transpiler("PASSED ALL TESTS\n");

	return 0;
}
