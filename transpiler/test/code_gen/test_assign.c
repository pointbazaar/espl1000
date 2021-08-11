#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "test_assign.h"

#include "util/test_statuscode.h"

static void status_test_assign(char* msg){
	printf(" - [TEST] %s\n", msg);
}

void test_assign(){

	status_test_assign("test assign");
	char* src = "fn main () ~> int { int x = 3; x = 4; return x; }";
	const int status = sourceToStatus(src);
	
	assert(status == 4);
}

void test_assign_plus(){

	status_test_assign("test assign +=");
	char* src = "fn main () ~> int { int x = 3; x += 4; return x; }";
	const int status = sourceToStatus(src);
	
	assert(status == 7);
}

void test_assign_minus(){

	status_test_assign("test assign -=");
	char* src = "fn main () ~> int { int x = 3; x -= 1; return x; }";
	const int status = sourceToStatus(src);
	
	assert(status == 2);
}

void test_assign_times(){

	status_test_assign("test assign *=");
	char* src = "fn main () ~> int { int x = 3; x *= 2; return x; }";
	const int status = sourceToStatus(src);
	
	assert(status == 6);
}

void test_assign_div(){

	status_test_assign("test assign /=");
	char* src = "fn main () ~> int { int x = 4; x /= 2; return x; }";
	const int status = sourceToStatus(src);
	
	assert(status == 2);
}

void test_assign_bitwise_and(){

	status_test_assign("test assign &=");
	char* src = "fn main () ~> int { int x = 0b01001111; x &= 0b00001010; return x; }";
	const int status = sourceToStatus(src);
	assert(status == 0b00001010);
}

void test_assign_bitwise_or(){

	status_test_assign("test assign |=");
	char* src = "fn main () ~> int { int x = 0b00001010; x |= 0b00000001; return x; }";
	const int status = sourceToStatus(src);
	assert(status == 0b00001011);
}

void test_assign_bitwise_shift_left(){

	status_test_assign("test assign <<=");
	char* src = "fn main () ~> int { int x = 0b00001010; x <<= 1; return x; }";
	const int status = sourceToStatus(src);
	assert(status == 0b00010100);
}

void test_assign_bitwise_shift_right(){

	status_test_assign("test assign >>=");
	char* src = "fn main () ~> int { int x = 0b00001010; x >>= 1; return x; }";
	const int status = sourceToStatus(src);
	assert(status == 0b00000101);
}
