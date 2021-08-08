#include <stdio.h>
#include <assert.h>

#include "test_other.h"
#include "../util/test_statuscode.h"


static void status_test_other(char* msg){
	printf(" - [TEST] %s\n", msg);
}

void test_statuscode(bool debug){

	status_test_other("statuscode");
	
	char* src = "fn main () ~> int { return 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 3);
}

void test_simplevar(bool debug){

	status_test_other("simplevar");
	
	char* src = "fn main () ~> int { int x = 2; return x; }";
	
	assert(sourceToStatus(src, debug) == 2);
}

void test_recursive(bool debug){

	status_test_other("recursive");
	
	char* src = "fn main () ~> int { return subr(8); } fn subr (int n) ~> int { if n > 0 { return subr(n-1); } return n; }";
	
	assert(sourceToStatus(src, debug) == 0);
}

void test_charconst_cmp(bool debug){

	status_test_other("charconst_cmp");
	
	char* src = "fn main () ~> int { if ('h' == 'h') {return 1;} return 0; } ";
	
	assert(sourceToStatus(src, debug));
}

void test_wrapped_expr(bool debug){

	status_test_other("wrapped_expr");
	
	char* src = "fn main () ~> int { return (8 + 8) / 2; } ";
	
	assert(sourceToStatus(src, debug) == 8);
}

