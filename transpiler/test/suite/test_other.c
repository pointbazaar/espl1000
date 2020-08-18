#include <stdio.h>

#include "test_other.h"
#include "../teststatuscode.h"

bool test_statuscode(bool debug){
	
	if(debug){ printf("test_statuscode\n"); }
	
	char* src = "fn main () ~> PInt { return 3; }";
	
	const int status = sourceToStatus(src, debug);
	
	return status == 3;
}

bool test_simplevar(bool debug){
	
	if(debug){ printf("test_simplevar\n"); }
	
	char* src = "fn main () ~> PInt { PInt x = 2; return x; }";
	
	return sourceToStatus(src, debug) == 2;
}

bool test_ifstmt(bool debug){
	
	if(debug){ printf("test_ifstmt\n"); }
	
	char* src = "fn main () ~> PInt { PInt x = 2; if x == 2 { return 3;} else { return 4; } }";
	
	return sourceToStatus(src, debug) == 3;
}

bool test_whilestmt(bool debug){
	
	if(debug){ printf("test_whilestmt\n"); }
	
	char* src = "fn main () ~> PInt { PInt x = 0; while x < 3 { x = x + 1; } return x; }";
	
	return sourceToStatus(src, debug) == 3;
}

bool test_subrcall(bool debug){
	
	if(debug){ printf("test_subrcall\n"); }
	
	char* src = "fn main () ~> PInt { return subr(); } fn subr () ~> PInt { return 9; }";
	
	return sourceToStatus(src, debug) == 9;
}

bool test_recursive(bool debug){
	
	if(debug){ printf("test_recursive\n"); }
	
	char* src = "fn main () ~> PInt { return subr(8); } fn subr (PInt n) ~> PInt { if n > 0 { return subr(n-1); } return n; }";
	
	return sourceToStatus(src, debug) == 0;
}

