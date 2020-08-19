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

int test_all(bool debug){
	
	printf("Running tests for smalldragon/transpiler:\n");
	
	const uint16_t testsRun = 
		TEST_COUNT_OP + TEST_COUNT_OTHER;
		
	uint16_t testsPassed 	= 0;
	
	bool (*tests[TEST_COUNT_OP + TEST_COUNT_OTHER])(bool debug);
	
	//first the ones from test_op.h
	tests[0] = test_add;
	tests[1] = test_sub;
	tests[2] = test_mul;
	tests[3] = test_div;
	tests[4] = test_mod;
	tests[5] = test_precedence;
	tests[6] = test_or;
	tests[7] = test_and;
	tests[8] = test_not;
	tests[9] = test_greater;
	tests[10] = test_lesser;
	tests[11] = test_geq;
	tests[12] = test_leq;
	tests[13] = test_eq;
	tests[14] = test_neq;
	
	// then the ones from test_other.h
	tests[15] = test_statuscode;
	tests[16] = test_simplevar;
	tests[17] = test_ifstmt;
	tests[18] = test_whilestmt;
	tests[19] = test_subrcall;
	tests[20] = test_recursive;
	
	for(int i=0; i < 2; i++){
		testsPassed += (*tests[i])(debug);
		
		if(testsPassed != i+1){
			printf("Test Failure\n");
			break;
		}
	}
	
	/*
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
	*/
	
	printf("\nPassed %d of %d Tests\n",testsPassed,testsRun);
	
	if(testsPassed != testsRun){ return 1; }
	
	return 0;
}
