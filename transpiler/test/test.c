#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test.h"
#include "teststatuscode.h"
#include "../main/transpiler.h"
#include "../main/flags.h"

bool test_statuscode(bool debug);
bool test_simplevar(bool debug);

void test_all(bool debug){
	
	printf("Running tests for smalldragon/transpiler:\n");
	
	const uint16_t testsRun    = 2;
	uint16_t testsPassed = 0;
	
	testsPassed += test_statuscode(debug);
	testsPassed += test_simplevar(debug);
	
	printf("\nPassed %d of %d Tests\n",testsPassed,testsRun);
}


// TESTS START

bool test_statuscode(bool debug){
	if(debug){ printf("test_statuscode\n"); }
	
	char* src = "fn main () ~> PInt { return 3; }";
	
	const int status = sourceToStatus(src);
	
	return status == 3;
}

bool test_simplevar(bool debug){
	if(debug){ printf("test_simplevar\n"); }
	
	char* src = "fn main () ~> PInt { PInt x = 2; return x; }";
	
	return sourceToStatus(src) == 2;
}
