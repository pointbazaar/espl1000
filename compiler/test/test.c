#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "typechecker/test_typechecker.h"
#include "typeinference/test_typeinference.h"
#include "avr_code_gen/test_avr_code_gen.h"
#include "x86_code_gen/test_x86_code_gen.h"
#include "gen_tac/test_gen_tac.h"

#include "lexer/test/test.h"
#include "parser/test/commandline/test.h"
#include "ast/test/test_ast.h"
#include "tac/test/test.h"
#include "rat/test.h"
#include "token/test/test.h"

static void status_test_transpiler(char* msg) {
	printf("[Compiler][TEST-SUITE] %s\n", msg);
}

struct sd_test_flags {
	bool testcase_duration;
};

static void parse_flags(int argc, char* argv[], struct sd_test_flags* flags) {

	flags->testcase_duration = false;

	for (int i = 1; i < argc; i++) {
		char* flag = argv[i];

		if (strcmp(flag, "--testcase-duration") == 0) {
			flags->testcase_duration = true;
		}
	}
}

#include "testcases.c"

struct testsuite {
	char* name;
	void (**testcases)();
};

struct testsuite testsuites[] = {
    {
	.name = "Typeinference",
	.testcases = tests_typeinference,
    },
    {
	.name = "Typechecker",
	.testcases = tests_typechecker,
    },
    {
	.name = "AVR CodeGen",
	.testcases = tests_avr_codegen,
    },
    {
	.name = "TAC CodeGen from Snippets",
	.testcases = tests_tac_codegen,
    },
    {
	.name = "x86 CodeGen",
	.testcases = tests_x86_codegen,
    },
    {
	.name = "Liveness",
	.testcases = tests_liveness,
    },
    {
	.name = NULL,
    }};

static void run_testcase(void (*testcase)(), struct sd_test_flags* flags) {

	struct timeval start, stop;

	gettimeofday(&start, NULL);
	testcase();
	gettimeofday(&stop, NULL);

	long elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;

	if (flags->testcase_duration) {
		printf("   took %4ld ms\n", elapsed_ms);
	}
}

// returns the number of tests that ran
static uint64_t run_testsuite(struct testsuite* suite, struct sd_test_flags* flags) {

	status_test_transpiler(suite->name);
	uint64_t ntests = 0;

	for (int i = 0; suite->testcases[i] != NULL; i++) {
		run_testcase(suite->testcases[i], flags);
		ntests++;
	}

	return ntests;
}

// returns the number of tests that ran
uint64_t run_testsuites(struct sd_test_flags* flags) {

	uint64_t ntests = 0;

	for (int suitei = 0; testsuites[suitei].name != NULL; suitei++) {

		ntests += run_testsuite(&testsuites[suitei], flags);
	}

	return ntests;
}

int main(int argc, char* argv[]) {

	struct sd_test_flags flags;
	struct timeval start, stop;

	parse_flags(argc, argv, &flags);

	status_test_transpiler("Running tests for smalldragon/compiler:");

	gettimeofday(&start, NULL);

	test_lexer_main();
	test_parser_main();
	test_token_main();
	test_rat_main();
	test_tac_main();
	test_ast_main();

	const uint64_t ntests = run_testsuites(&flags);

	gettimeofday(&stop, NULL);

	long elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;

	char* status_msg;
	asprintf(&status_msg, "took %ld ms", elapsed_ms);
	status_test_transpiler(status_msg);

	asprintf(&status_msg, "completed %ld tests", ntests);
	status_test_transpiler(status_msg);

	status_test_transpiler("PASSED ALL TESTS");

	free(status_msg);

	return 0;
}
