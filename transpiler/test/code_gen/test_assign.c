#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "test_assign.h"

#include "util/test_statuscode.h"

void test_assign(bool debug){
	
	if(debug){ printf("test_assign\n"); }
	char* src = "fn main () ~> uint { int x = 3; x = 4; return x; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 4);
}

void test_assign_plus(bool debug){
	
	if(debug){ printf("test_assign_plus\n"); }
	char* src = "fn main () ~> uint { int x = 3; x += 4; return x; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 7);
}

void test_assign_minus(bool debug){
	
	if(debug){ printf("test_assign\n"); }
	char* src = "fn main () ~> uint { int x = 3; x -= 1; return x; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 2);
}

void test_assign_times(bool debug){
	
	if(debug){ printf("test_assign_times\n"); }
	char* src = "fn main () ~> uint { int x = 3; x *= 2; return x; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 6);
}

void test_assign_div(bool debug){
	
	if(debug){ printf("test_assign_div\n"); }
	char* src = "fn main () ~> uint { int x = 4; x /= 2; return x; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 2);
}
