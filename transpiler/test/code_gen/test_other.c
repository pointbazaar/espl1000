#include <stdio.h>
#include <assert.h>

#include "test_other.h"
#include "../util/test_statuscode.h"

void test_statuscode(bool debug){
	
	if(debug){ printf("test_statuscode\n"); }
	
	char* src = "fn main () ~> int { return 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 3);
}

void test_simplevar(bool debug){
	
	if(debug){ printf("test_simplevar\n"); }
	
	char* src = "fn main () ~> int { uint x = 2; return x; }";
	
	assert(sourceToStatus(src, debug) == 2);
}

void test_recursive(bool debug){
	
	if(debug){ printf("test_recursive\n"); }
	
	char* src = "fn main () ~> int { return subr(8); } fn subr (uint n) ~> uint { if n > 0 { return subr(n-1); } return n; }";
	
	assert(sourceToStatus(src, debug) == 0);
}

void test_charconst_cmp(bool debug){

	if(debug){ printf("test_charconst_cmp\n"); }
	
	char* src = "fn main () ~> int { return ('h' == 'h'); } ";
	
	assert(sourceToStatus(src, debug));
}

void test_wrapped_expr(bool debug){
	
	if(debug){ printf("test_wrapped_expr\n"); }
	
	char* src = "fn main () ~> int { return (8 + 8) / 2; } ";
	
	assert(sourceToStatus(src, debug) == 8);
}

