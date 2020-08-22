#include <stdio.h>

#include "test_op.h"
#include "../teststatuscode.h"

bool test_add(bool debug){
	
	if(debug){ printf("test_add\n"); }
	
	char* src = "fn main () ~> PInt { return 3+4; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 7;
}

bool test_sub(bool debug){
	
	if(debug){ printf("test_sub\n"); }
	
	char* src = "fn main () ~> PInt { return 6-4; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 2;
}

bool test_mul(bool debug){
	
	if(debug){ printf("test_mul\n"); }
	
	char* src = "fn main () ~> PInt { return 2 * 4; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 8;
}

bool test_div(bool debug){
	
	if(debug){ printf("test_div\n"); }
	
	char* src = "fn main () ~> PInt { return 4 / 2; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 2;
}

bool test_mod(bool debug){
	
	if(debug){ printf("test_mod\n"); }
	
	char* src = "fn main () ~> PInt { return 4 % 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 1;
}

bool test_precedence(bool debug){
	
	if(debug){ printf("test_precedence\n"); }
	
	char* src = "fn main () ~> PInt { return 3 + 4 / 2; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 5;
}

bool test_or(bool debug){
	
	if(debug){ printf("test_or\n"); }
	
	char* src = "fn main () ~> PInt { if true || false { return 3; } return 2; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 3;
}

bool test_and(bool debug){
	
	if(debug){ printf("test_and\n"); }
	
	char* src = "fn main () ~> PInt { return true && false; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 0;
}

bool test_not(bool debug){
	
	if(debug){ printf("test_not\n"); }
	
	char* src = "fn main () ~> PInt { return ! false; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 1;
}

//comparison tests

bool test_greater(bool debug){
	
	if(debug){ printf("test_greater\n"); }
	
	char* src = "fn main () ~> PInt { return 4 > 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status != 0;
}

bool test_lesser(bool debug){
	
	if(debug){ printf("test_lesser\n"); }
	
	char* src = "fn main () ~> PInt { return 4 < 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 0;
}

bool test_geq(bool debug){
	
	if(debug){ printf("test_geq\n"); }
	
	char* src = "fn main () ~> PInt { return 3 >= 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 0;
}

bool test_leq(bool debug){
	
	if(debug){ printf("test_leq\n"); }
	
	char* src = "fn main () ~> PInt { return 2 <= 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 0;
}

bool test_eq(bool debug){
	
	if(debug){ printf("test_eq\n"); }
	
	char* src = "fn main () ~> PInt { return 8 == 8; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 1;
}

bool test_neq(bool debug){
	
	if(debug){ printf("test_neq\n"); }
	
	char* src = "fn main () ~> PInt { return 4 != 4; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status != 0;
}

bool test_chained_cmp(bool debug){
	
	if(debug){ printf("test_chained_cmp\n"); }
	
	char* src = "fn main () ~> PInt { return 3 < 4 < 5; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status != 0;
}
