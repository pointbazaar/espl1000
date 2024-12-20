#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "test.h"

static uint32_t test_num = 1;

void printt2(char* test_name) {
	printf("[Token][Test][#%02d] %s", test_num++, test_name);
}

int test_token_main() {

	printf("[Token] Running Tests... \n");

	// TODO: write more tests
	test_tokenlist_no_leak();

	test_tokenlist_1element();

	test_tokenlist_copy();

	test_tokenlist_code();

	test_tokenlist_print();

	test_tokenlist_consume();

	printf("[Token] All %i Tests Passed\n", test_num - 1);

	return 0;
}
