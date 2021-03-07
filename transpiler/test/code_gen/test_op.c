#include <stdio.h>
#include <assert.h>

#include "test_op.h"

#include "util/test_statuscode.h"

void test_add(bool debug){
	
	if(debug){ printf("test_add\n"); }
	
	char* src = "fn main () ~> uint { return 3+4; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 7);
}

void test_sub(bool debug){
	
	if(debug){ printf("test_sub\n"); }
	
	char* src = "fn main () ~> uint { return 6-4; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 2);
}

void test_mul(bool debug){
	
	if(debug){ printf("test_mul\n"); }
	
	char* src = "fn main () ~> uint { return 2 * 4; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 8);
}

void test_div(bool debug){
	
	if(debug){ printf("test_div\n"); }
	
	char* src = "fn main () ~> uint { return 4 / 2; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 2);
}

void test_mod(bool debug){
	
	if(debug){ printf("test_mod\n"); }
	
	char* src = "fn main () ~> uint { return 4 % 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 1);
}

void test_precedence(bool debug){
	
	if(debug){ printf("test_precedence\n"); }
	
	char* src = "fn main () ~> uint { return 3 + 4 / 2; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 5);
}

void test_or(bool debug){
	
	if(debug){ printf("test_or\n"); }
	
	char* src = "fn main () ~> uint { if true || false { return 3; } return 2; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 3);
}

void test_and(bool debug){
	
	if(debug){ printf("test_and\n"); }
	
	char* src = "fn main () ~> uint { return true && false; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 0);
}

void test_not(bool debug){
	
	if(debug){ printf("test_not\n"); }
	
	char* src = "fn main () ~> uint { return ! false; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 1);
}

//comparison tests

void test_greater(bool debug){
	
	if(debug){ printf("test_greater\n"); }
	
	char* src = "fn main () ~> uint { return 4 > 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status != 0);
}

void test_lesser(bool debug){
	
	if(debug){ printf("test_lesser\n"); }
	
	char* src = "fn main () ~> uint { return 4 < 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 0);
}

void test_geq(bool debug){
	
	if(debug){ printf("test_geq\n"); }
	
	char* src = "fn main () ~> uint { return 3 >= 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 1);
}

void test_leq(bool debug){
	
	if(debug){ printf("test_leq\n"); }
	
	char* src = "fn main () ~> uint { return 2 <= 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 1);
}

void test_eq(bool debug){
	
	if(debug){ printf("test_eq\n"); }
	
	char* src = "fn main () ~> uint { return 8 == 8; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 1);
}

void test_neq(bool debug){
	
	if(debug){ printf("test_neq\n"); }
	
	char* src = "fn main () ~> uint { return 4 != 4; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 0);
}

void test_chained_cmp(bool debug){
	
	if(debug){ printf("test_chained_cmp\n"); }
	
	char* src = "fn main () ~> uint { return 3 > 4 < 5; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 0);
}

void test_bitwise_and(bool debug){
	if(debug){ printf("test_bitwise_and\n"); }
	
	char* src = "fn main () ~> uint { return 3 & 1; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 1);
}

void test_bitwise_or(bool debug){
	if(debug){ printf("test_bitwise_or\n"); }
	
	char* src = "fn main () ~> uint { return 1 | 2; }";
	
	const int status = sourceToStatus(src, debug);
	
	assert(status == 3);
}

void test_bitwise_leftshift(bool debug){
	if(debug){ printf("test_bitwise_leftshift\n"); }
	char* src = "fn main () ~> uint { return 1 << 1; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 2);
}

void test_bitwise_rightshift(bool debug){
	if(debug){ printf("test_bitwise_rightshift\n"); }
	char* src = "fn main () ~> uint { return 2 >> 1; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 1);
}

void test_bitwise_xor(bool debug){
	if(debug){ printf("test_bitwise_xor\n"); }
	char* src = "fn main () ~> uint { return 8 ^ 5; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 13);
}

void test_bitwise_neg(bool debug){
	if(debug){ printf("test_bitwise_neg\n"); }
	char* src = "fn main () ~> uint { return ~4 + 5; }";
	const int status = sourceToStatus(src, debug);
	
	assert(status == 0);
}
