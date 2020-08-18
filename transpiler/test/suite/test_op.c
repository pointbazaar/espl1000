#include <stdio.h>

#include "test_op.h"
#include "../teststatuscode.h"

bool test_add(bool debug){
	
	if(debug){ printf("test_add\n"); }
	
	char* src = "fn main () ~> PInt { return 3+4; }";
	
	const int status = sourceToStatus(src);
	
	return status == 7;
}

bool test_sub(bool debug){
	
	if(debug){ printf("test_sub\n"); }
	
	char* src = "fn main () ~> PInt { return 6-4; }";
	
	const int status = sourceToStatus(src);
	
	return status == 2;
}

bool test_mul(bool debug){
	
	if(debug){ printf("test_mul\n"); }
	
	char* src = "fn main () ~> PInt { return 2 * 4; }";
	
	const int status = sourceToStatus(src);
	
	return status == 8;
}

bool test_div(bool debug){
	
	if(debug){ printf("test_div\n"); }
	
	char* src = "fn main () ~> PInt { return 4 / 2; }";
	
	const int status = sourceToStatus(src);
	
	return status == 2;
}

bool test_precedence(bool debug){
	
	if(debug){ printf("test_precedence\n"); }
	
	char* src = "fn main () ~> PInt { return 3 + 4 / 2; }";
	
	const int status = sourceToStatus(src);
	
	return status == 5;
}

bool test_or(bool debug){
	
	if(debug){ printf("test_or\n"); }
	
	char* src = "fn main () ~> PInt { if true or false { return 3; } return 2; }";
	
	const int status = sourceToStatus(src);
	
	return status == 3;
}

bool test_and(bool debug){
	
	if(debug){ printf("test_and\n"); }
	
	char* src = "fn main () ~> PInt { return true && false; }";
	
	const int status = sourceToStatus(src);
	
	return status == 0;
}

bool test_not(bool debug){
	
	if(debug){ printf("test_not\n"); }
	
	char* src = "fn main () ~> PInt { return !false; }";
	
	const int status = sourceToStatus(src);
	
	return status == 1;
}
