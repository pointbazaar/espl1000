#include <stdio.h>
#include <assert.h>

#include "test_op.h"

#include "util/test_statuscode.h"

static void status_test_op(char* msg){
	printf(" - [TEST] %s\n", msg);
}

void test_add(){

	status_test_op("test_add");
	
	char* src = "fn main () ~> int { return 3+4; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 7);
}

void test_sub(){

	status_test_op("test_sub");
	
	char* src = "fn main () ~> int { return 6-4; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 2);
}

void test_mul(){

	status_test_op("test_mul");
	
	char* src = "fn main () ~> int { return 2 * 4; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 8);
}

void test_div(){

	status_test_op("test_div");
	
	char* src = "fn main () ~> int { return 4 / 2; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 2);
}

void test_mod(){

	status_test_op("test_mod");
	
	char* src = "fn main () ~> int { return 4 % 3; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 1);
}

void test_precedence(){

	status_test_op("test_precedence");
	
	char* src = "fn main () ~> int { return 3 + 4 / 2; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 5);
}

void test_or(){

	status_test_op("test_or");
	
	char* src = "fn main () ~> int { if true || false { return 3; } return 2; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 3);
}

void test_and(){

	status_test_op("test_and");
	
	char* src = "fn main () ~> int { if true && false { return 1; } return 0;  }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 0);
}

void test_not(){

	status_test_op("test_not");
	
	char* src = "fn main () ~> int { if !false { return 1; } return 0; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 1);
}

//comparison tests

void test_greater(){

	status_test_op("test_greater");
	
	char* src = "fn main () ~> int { if 4 > 3 { return 1; } return 0; }";
	
	const int status = sourceToStatus(src);
	
	assert(status != 0);
}

void test_lesser(){

	status_test_op("test_lesser");
	
	char* src = "fn main () ~> int { if 4 < 3 { return 1; } return 0; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 0);
}

void test_geq(){

	status_test_op("test_geq");
	
	char* src = "fn main () ~> int { if 3 >= 3 { return 1; } return 0; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 1);
}

void test_leq(){

	status_test_op("test_leq");
	
	char* src = "fn main () ~> int { if 2 <= 3 { return 1; } return 0; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 1);
}

void test_eq(){

	status_test_op("test_eq");
	
	char* src = "fn main () ~> int { if 8 == 8 { return 1; } return 0; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 1);
}

void test_neq(){

	status_test_op("test_neq");
	
	char* src = "fn main () ~> int { if 4 != 4 { return 1; } return 0; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 0);
}

void test_chained_cmp(){

	status_test_op("test_chained_cmp");
	
	char* src = "fn main () ~> int { if 3 > 4 < 5 { return 1; } return 0; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 0);
}

void test_bitwise_and(){

	status_test_op("test_bitwise_and");
	
	char* src = "fn main () ~> int { return 3 & 1; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 1);
}

void test_bitwise_or(){

	status_test_op("test_bitwise_or");
	
	char* src = "fn main () ~> int { return 1 | 2; }";
	
	const int status = sourceToStatus(src);
	
	assert(status == 3);
}

void test_bitwise_leftshift(){

	status_test_op("test_bitwise_leftshift");
	
	char* src = "fn main () ~> int { return 1 << 1; }";
	const int status = sourceToStatus(src);
	
	assert(status == 2);
}

void test_bitwise_rightshift(){

	status_test_op("test_bitwise_rightshift");
	
	char* src = "fn main () ~> int { return 2 >> 1; }";
	const int status = sourceToStatus(src);
	
	assert(status == 1);
}

void test_bitwise_xor(){

	status_test_op("test_bitwise_xor");
	
	char* src = "fn main () ~> int { return 8 ^ 5; }";
	const int status = sourceToStatus(src);
	
	assert(status == 13);
}

void test_bitwise_neg(){

	status_test_op("test_bitwise_neg");
	
	char* src = "fn main () ~> int { return ~4 + 5; }";
	const int status = sourceToStatus(src);
	
	assert(status == 0);
}
