#include <stdio.h>
#include <stdbool.h>

#include "test_assign.h"
#include "../teststatuscode.h"

bool test_assign(bool debug){
	
	if(debug){ printf("test_assign\n"); }
	char* src = "fn main () ~> PInt { Int x = 3; x = 4; return x; }";
	const int status = sourceToStatus(src, debug);
	return status == 4;
}

bool test_assign_plus(bool debug){
	
	if(debug){ printf("test_assign_plus\n"); }
	char* src = "fn main () ~> PInt { Int x = 3; x += 4; return x; }";
	const int status = sourceToStatus(src, debug);
	return status == 7;
}

bool test_assign_minus(bool debug){
	
	if(debug){ printf("test_assign\n"); }
	char* src = "fn main () ~> PInt { Int x = 3; x -= 1; return x; }";
	const int status = sourceToStatus(src, debug);
	return status == 2;
}

bool test_assign_times(bool debug){
	
	if(debug){ printf("test_assign_times\n"); }
	char* src = "fn main () ~> PInt { Int x = 3; x *= 2; return x; }";
	const int status = sourceToStatus(src, debug);
	return status == 6;
}

bool test_assign_div(bool debug){
	
	if(debug){ printf("test_assign_div\n"); }
	char* src = "fn main () ~> PInt { Int x = 4; x /= 2; return x; }";
	const int status = sourceToStatus(src, debug);
	return status == 2;
}
