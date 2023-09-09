#include <stdio.h>

#include "test_str_ast.h"

FILE* file;

int main() {

	file = fopen("/tmp/test", "w+");

	printf("[AST][TEST] running AST str Tests...\n");

	//--- from test_str_ast.c ---
	test_str_binconst();
	test_str_intconst();
	test_str_charconst();
	test_str_hexconst();

	test_str_expr();
	test_str_op();
	test_str_unopterm();
	test_str_term();

	printf("[AST Module] Passed All Tests\n");

	fclose(file);

	return 0;
}
