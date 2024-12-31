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

static void status_test_transpiler(char* msg) {
	printf("[Compiler][TEST-SUITE] %s\n", msg);
}

struct sd_test_flags {
	// print testcase duration
	bool testcase_duration;
	// print help
	bool help;
	// list testsuites
	bool testsuites;
	// run single test suite
	char* testsuite;
};

static void print_help() {
	printf("OPTIONS:\n");
	printf("-help\n");
	printf("-testsuite <SUITE>\n");
	printf("-testsuites\n");
	printf("-testcase-duration\n");
}

static int parse_flags(int argc, char* argv[], struct sd_test_flags* flags) {

	flags->testcase_duration = false;
	flags->help = false;
	flags->testsuite = NULL;
	flags->testsuites = false;

	for (int i = 1; i < argc; i++) {
		char* flag = argv[i];

		if (strcmp(flag, "-testcase-duration") == 0) {
			flags->testcase_duration = true;
		}
		if (strcmp(flag, "-help") == 0) {
			flags->help = true;
		}
		if (strcmp(flag, "-testsuite") == 0) {
			if (i + 1 >= argc) {
				fprintf(stderr, "need testsuite argument\n");
				return 1;
			}
			flags->testsuite = argv[i + 1];
		}
		if (strcmp(flag, "-testsuites") == 0) {
			flags->testsuites = true;
		}
	}
	return 0;
}

#include "testcases.c"

struct testsuite {
	char* name;
	void (**testcases)();
};

struct testsuite testsuites[] = {
    {
	.name = "Token",
	.testcases = tests_token,
    },
    {
	.name = "Lexer",
	.testcases = tests_lexer,
    },
    {
	.name = "AST",
	.testcases = tests_ast,
    },
    {
	.name = "Parser",
	.testcases = tests_parser,
    },
    {
	.name = "RAT",
	.testcases = tests_rat,
    },
    {
	.name = "TAC",
	.testcases = tests_tac,
    },
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
	.name = "Tables",
	.testcases = tests_tables,
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

		char* name = testsuites[suitei].name;

		if (flags->testsuite != NULL) {
			if (strcmp(flags->testsuite, name) != 0) {
				continue;
			}
		}

		ntests += run_testsuite(&testsuites[suitei], flags);
	}

	return ntests;
}

static void print_testsuites() {

	for (int suitei = 0; testsuites[suitei].name != NULL; suitei++) {

		char* name = testsuites[suitei].name;

		printf("%s\n", name);
	}
}

static int validate_flags(struct sd_test_flags* flags) {

	if (flags->testsuite == NULL) {
		return 0;
	}

	for (int suitei = 0; testsuites[suitei].name != NULL; suitei++) {

		char* name = testsuites[suitei].name;

		if (strcmp(flags->testsuite, name) == 0) {
			return 0;
			;
		}
	}

	fprintf(stderr, "'%s' did not match any testsuite\n", flags->testsuite);

	return 1;
}

int main(int argc, char* argv[]) {

	int status = 0;
	struct sd_test_flags flags;
	struct timeval start, stop;

	status = parse_flags(argc, argv, &flags);

	if (status != 0) {
		return 1;
	}

	status = validate_flags(&flags);

	if (status != 0) {
		return 1;
	}

	if (flags.help) {
		print_help();
		return 0;
	}

	if (flags.testsuites) {
		print_testsuites();
		return 0;
	}

	status_test_transpiler("Running tests for smalldragon/compiler:");

	gettimeofday(&start, NULL);

	const uint64_t ntests = run_testsuites(&flags);

	gettimeofday(&stop, NULL);

	long elapsed_ms = (stop.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (stop.tv_usec - start.tv_usec) / 1000.0;

	char* status_msg;
	asprintf(&status_msg, "took %ld ms", elapsed_ms);
	status_test_transpiler(status_msg);
	free(status_msg);

	asprintf(&status_msg, "completed %ld tests", ntests);
	status_test_transpiler(status_msg);

	status_test_transpiler("PASSED ALL TESTS");

	free(status_msg);

	return 0;
}
