#include <stdio.h>
#include <stdbool.h>

#include "typechecker/test_typechecker.h"
#include "typeinference/test_typeinference.h"

static void status_test_transpiler(char* msg){
	printf("[TRANSPILER][TEST-SUITE] %s\n", msg);
}

int main(){

	status_test_transpiler("Running tests for smalldragon/transpiler:");

	status_test_transpiler("Typeinference");
	test_suite_typeinference();

	status_test_transpiler("Typechecker");
	test_suite_typechecker();

	status_test_transpiler("PASSED ALL TESTS\n");

	return 0;
}

