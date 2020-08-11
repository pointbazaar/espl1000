#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test.h"
#include "../main/transpiler.h"
#include "../main/flags.h"

bool test_statuscode(bool debug);

void test_all(bool debug){
	
	printf("running tests for smalldragon/transpiler:\n");
	uint16_t testsRun    = 0;
	uint16_t testsPassed = 0;
	
	const uint16_t num_tests_max = 1;
	
	bool (*tests[num_tests_max]) (bool debug);
	
	tests[0] = test_statuscode;
	
	for(int i=0;i < num_tests_max; i++){
		testsPassed += tests[i](debug);
		testsRun++;
		if(testsPassed < testsRun){
			printf("last test did not pass!\n");
			if(!debug){
				printf("Isolating the failing Test:\n");
				printf("------------------------------\n");
				
				tests[i](true);
			}
			break;
		}
	}
	
	printf("passed %d of %d(this run) of %d(total test suite) \n",testsPassed,testsRun, num_tests_max);
}


// TESTS START

bool test_statuscode(bool debug){
	
	char* src = "fn main () ~> PInt { printf(\"test_statuscode\"); return 3; }";
	
	//write this to a file
	FILE* file = fopen("test.dg","w");
	fprintf(file, "%s", src);
	fclose(file);
	
	//TODO: transpile it
	struct Flags* flags = malloc(sizeof(struct Flags));
	flags->debug = false;
	flags->avr = false;
	flags->test = false;
	char** gcc_flags = NULL;
	int gcc_flags_count = 0;
	
	transpileAndCompile("test.dg", flags, gcc_flags, gcc_flags_count);
	
	
	
	//TODO: delete that file
	remove("test.dg");
	remove("test.c");
	
	//TODO: test that the code, when transpiled,
	//returns a status code of 3
	system("./a.out");
	remove("a.out");
	
	free(flags);
	
	return true;
}
