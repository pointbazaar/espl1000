#include <stdio.h>
#include <stdbool.h>

#include "test_gen_tac.h"

void test_suite_tac_codegen(){
	
	test_gen_tac_mdirect();
	test_gen_tac_assignstmt();
}

void status_test_codegen_tac(char* msg){
    printf(" - [TEST] TAC CodeGen %s\n", msg);
}
