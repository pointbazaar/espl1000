#include <stdio.h>
#include <assert.h>

#include "test_stmt.h"
#include "../util/test_statuscode.h"

static void status_test_stmt(char* msg){
	printf(" - [TEST] %s\n", msg);
}

void test_ifstmt(bool debug){

	status_test_stmt("ifstmt");
	
	char* src = "fn main () ~> int { int x = 2; if x == 2 { return 3;} else { return 4; } }";
	
	assert(sourceToStatus(src, debug) == 3);
}

void test_whilestmt(bool debug){

	status_test_stmt("whilestmt");
	
	char* src = "fn main () ~> int { int x = 0; while x < 3 { x = x + 1; } return x; }";
	
	assert(sourceToStatus(src, debug) == 3);
}

void test_subrcall(bool debug){

	status_test_stmt("subrcall");
	
	char* src = "fn main () ~> int { return subr(); } fn subr () ~> int { return 9; }";
	
	assert(sourceToStatus(src, debug) == 9);
}

void test_break(bool debug){

	status_test_stmt("break");
	
	char* src = "fn main () ~> int { while true { break; return 3; } return 9; } ";
	
	assert(sourceToStatus(src, debug) == 9);
}

void test_continue(bool debug){

	status_test_stmt("continue");
	
	char* src = "fn main () ~> int { int x = 6; while x == 6 {  x += 1; if x == 7 { continue; } x -= 1; } return x; } ";
	
	assert(sourceToStatus(src, debug) == 7);
}
