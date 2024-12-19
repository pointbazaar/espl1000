#include "test.h"
#include <stdio.h>

void status(char* msg) {
	printf("[TAC][Test] %s\n", msg);
}

void test_tac_tostring();
void test_tac_opt_dest();

void test_tacbuffer_tostring();
void test_tacbuffer_indexof();
void test_tacbuffer_set();
void test_tacbuffer_get();
void test_tacbuffer_get_last();
void test_tacbuffer_last_dest();
void test_tacbuffer_count();

int main() {

	printf("[TAC] Running Tests...\n");

	test_tac_tostring();
	test_tac_opt_dest();

	test_tacbuffer_tostring();
	test_tacbuffer_indexof();
	test_tacbuffer_set();
	test_tacbuffer_get();
	test_tacbuffer_get_last();
	test_tacbuffer_last_dest();
	test_tacbuffer_count();

	return 0;
}
