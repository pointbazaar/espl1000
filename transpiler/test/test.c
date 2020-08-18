#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

#include "../main/transpiler.h"
#include "../main/flags.h"

#include "test.h"
#include "teststatuscode.h"

#include "suite/test_op.h"
#include "suite/test_other.h"

void test_all(bool debug){
	
	printf("Running tests for smalldragon/transpiler:\n");
	
	const uint16_t testsRun = 
		TEST_COUNT_OP + TEST_COUNT_OTHER;
		
	uint16_t testsPassed 	= 0;
	
	//6 regular tests
	
	testsPassed += test_statuscode(debug);
	testsPassed += test_simplevar(debug);
	testsPassed += test_ifstmt(debug);
	testsPassed += test_whilestmt(debug);
	testsPassed += test_subrcall(debug);
	testsPassed += test_recursive(debug);
	
	//5 operator related tests
	
	testsPassed += test_add(debug);
	testsPassed += test_sub(debug);
	testsPassed += test_mul(debug);
	testsPassed += test_div(debug);
	testsPassed += test_precedence(debug);
	
	printf("\nPassed %d of %d Tests\n",testsPassed,testsRun);
}
