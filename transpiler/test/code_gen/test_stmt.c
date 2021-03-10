#include <stdio.h>
#include <assert.h>

#include "test_stmt.h"
#include "../util/test_statuscode.h"

void test_ifstmt(bool debug){
	
	if(debug){ printf("test_ifstmt\n"); }
	
	char* src = "fn main () ~> int { uint x = 2; if x == 2 { return 3;} else { return 4; } }";
	
	assert(sourceToStatus(src, debug) == 3);
}

void test_whilestmt(bool debug){
	
	if(debug){ printf("test_whilestmt\n"); }
	
	char* src = "fn main () ~> int { uint x = 0; while x < 3 { x = x + 1; } return x; }";
	
	assert(sourceToStatus(src, debug) == 3);
}

void test_subrcall(bool debug){
	
	if(debug){ printf("test_subrcall\n"); }
	
	char* src = "fn main () ~> int { return subr(); } fn subr () ~> uint { return 9; }";
	
	assert(sourceToStatus(src, debug) == 9);
}

void test_break(bool debug){
	
	if(debug){ printf("test_break\n"); }
	
	char* src = "fn main () ~> int { while true { break; return 3; } return 9; } ";
	
	assert(sourceToStatus(src, debug) == 9);
}

void test_continue(bool debug){
	
	if(debug){ printf("test_continue\n"); }
	
	char* src = "fn main () ~> int { uint x = 6; while x == 6 {  x += 1; if x == 7 { continue; } x -= 1; } return x; } ";
	
	assert(sourceToStatus(src, debug) == 7);
}
