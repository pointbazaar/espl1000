#include <stdio.h>
#include <stdbool.h>

#include "test_gen_tac.h"

void test_suite_tac_codegen(){
	
	test_gen_tac_mdirect();
	test_gen_tac_assignstmt();
	test_gen_tac_massignstmt();
	test_gen_tac_expr();
	
	test_gen_tac_ifstmt();
	test_gen_tac_whilestmt();
	test_gen_tac_forstmt();
	
	test_gen_tac_simplevar();
	test_gen_tac_variable();
	
	test_gen_tac_call();
}

void status_test_codegen_tac(char* msg){
    printf(" - [TEST] TAC CodeGen %s\n", msg);
}
