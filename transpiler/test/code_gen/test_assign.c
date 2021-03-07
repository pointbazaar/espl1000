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

void test_assign_bitwise_and(bool debug){
	
	if(debug){ printf("test_assign_bitwise_and\n"); }
	char* src = "fn main () ~> uint { int x = 0b01001111; x &= 0b00001010; return x; }";
	const int status = sourceToStatus(src, debug);
	assert(status == 0b00001010);
}

void test_assign_bitwise_or(bool debug){
	
	if(debug){ printf("test_assign_bitwise_or\n"); }
	char* src = "fn main () ~> uint { int x = 0b00001010; x |= 0b00000001; return x; }";
	const int status = sourceToStatus(src, debug);
	assert(status == 0b00001011);
}

void test_assign_bitwise_shift_left(bool debug){
	
	if(debug){ printf("test_assign_shift_left\n"); }
	char* src = "fn main () ~> uint { int x = 0b00001010; x <<= 1; return x; }";
	const int status = sourceToStatus(src, debug);
	assert(status == 0b00010100);
}

void test_assign_bitwise_shift_right(bool debug){
	
	if(debug){ printf("test_assign_shift_right\n"); }
	char* src = "fn main () ~> uint { int x = 0b00001010; x >>= 1; return x; }";
	const int status = sourceToStatus(src, debug);
	assert(status == 0b00000101);
}
