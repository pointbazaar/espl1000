#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/astnodes/types/StructTypeTest.h>

#include "ParserTest.h"

#include "../astnodes/const/BoolConstTest.h"
#include "../astnodes/expr/ExprTest.h"
#include "../astnodes/NamespaceTest.h"
#include "../astnodes/var/SimpleVarTest.h"
#include "../astnodes/struct/StructMemberTest.h"
#include "../astnodes/var/VariableTest.h"
#include "../astnodes/statements/AssignStmtTest.h"
#include "../astnodes/const/CharConstTest.h"
#include "../astnodes/const/FloatConstTest.h"
#include "../astnodes/statements/CallTest.h"
#include "../astnodes/statements/RetStmtTest.h"
#include "../astnodes/statements/StmtTest.h"
#include "../astnodes/StmtBlockTest.h"
#include "../astnodes/types/SubrTypeTest.h"
#include "../astnodes/statements/WhileStmtTest.h"
#include "../astnodes/types/BasicTypeTest.h"
#include "../astnodes/subr/DeclArgTest.h"
#include "../astnodes/statements/IfStmtTest.h"
#include "../astnodes/subr/MethodTest.h"
#include "../astnodes/types/SimpleTypeTest.h"
#include "../astnodes/struct/StructDeclTest.h"
#include "../astnodes/expr/TermTest.h"
#include "../astnodes/RangeTest.h"
#include "../astnodes/statements/ForStmtTest.h"
#include "../astnodes/statements/SwitchStmtTest.h"
#include "../astnodes/statements/CaseStmtTest.h"
#include "../astnodes/statements/TryCatchStmtTest.h"
#include "../astnodes/LambdaTest.h"

#define FAILRET if(!passed){ printf("FAILED\n"); return false;}

//there are multiple test suites which test multiple aspects
//of the parser.

// --- START OF TEST SUITES ---

bool test_suite_constnodes(bool debug);
bool test_suite_term_expr_simplevar_var(bool debug);

bool test_suite_stmts(bool debug);
bool test_suite_assignstmt(bool debug);

bool test_suite_method(bool debug);
bool test_suite_struct(bool debug);
bool test_suite_types(bool debug);
bool test_suite_range(bool debug);

// --- END OF TEST SUITES ---

static void status(char* msg){
	printf("[Parser][TEST-SUITE] %s\n", msg);
}

int test_all(bool debug) {

    printf("[Parser Module] running Parser Tests...\n");

    //if some tests fail, the test suit should be
    //stopped right there. no use running other tests
    //if one test already failed. we can work
    //on the failed test then.

    //execute all tests and give feedback
    //every test should stdout its name, such that failing tests can be identified easily

    //the tests should start with the easy tests first

    if(!test_suite_constnodes(debug)) {
        printf("suite_constnodes failed\n");
        return false;
    }
    if(!test_suite_term_expr_simplevar_var(debug)) {
        printf("suite_term_expr_simplevar_var failed\n");
        return false;
    }
    if(!test_suite_stmts(debug)) {
        printf("suite_stmts failed\n");
        return false;
    }
    if(!test_suite_assignstmt(debug)){
		printf("suite_assignstmt failed\n");
		return false;
	}
    if(!test_suite_method(debug)) {
        printf("suite_method failed\n");
        return false;
    }
    if(!test_suite_struct(debug)) {
        printf("suite_struct failed\n");
        return false;
    }
    if(!test_suite_types(debug)) {
        printf("suite_types failed\n");
        return false;
    }
    if(!test_suite_range(debug)){
		printf("suit_range failed\n");
		return false;
	}
	
	test_switch(debug);
	test_parser_case_stmt(debug);

	test_lambda(debug);

    printf("[Parser Module] PASSED ALL TESTS\n");

    return 0;
}

// --- TEST SUITE IMPLEMENTATIONS ---


bool test_suite_constnodes(bool debug) {
	
	status("test_suite_constnodes"); 

    bool passed = true;

    passed &=  boolconst_test_parse_bool_constant_node(debug);
	FAILRET
    passed &=  charconst_test_parse_char_constant_node(debug);
    FAILRET
    passed &=  charconst_test_parse_char_constant_node_newline(debug);
    FAILRET

    passed &=  floatconst_test1(debug);
    FAILRET
    passed &=  floatconst_test2(debug);
	FAILRET
    return passed;
}

bool test_suite_term_expr_simplevar_var(bool debug) {
	
	status("test_suite_term_expr_simplevar_var");

    int count  = 13;
    int passed = 0;

    passed +=  term_test_simple_term(debug);
    passed +=  term_test_variable_term(debug);
    passed +=  term_test_parentheses(debug);

    passed +=  expr_recognize_2_op_expr(debug);
    passed +=  expr_test_simple_expression(debug);
    passed +=  expr_test_variable_name_expression(debug);

    passed +=  expr_test_comparison(debug);

    passed +=  simplevar_test_parse_simple_indexed_variable(debug);
    passed +=  simplevar_test_parse_simple_variable(debug);
    passed +=  simplevar_test_2_indices(debug);

    passed +=  variable_test_parse_index_access(debug);
    passed +=  variable_test_parse_struct_member_access(debug);
    passed +=  variable_test_parse_struct_member_access_and_index_access(debug);

    return passed == count;
}

bool test_suite_stmts(bool debug) {

	status("test_suite_stmts");

    retstmt_test1(debug);
    retstmt_test2(debug);
	retstmt_test3(debug);

    methodcall_test1(debug);
    methodcall_test2(debug);
    methodcall_test3(debug);
    methodcall_test_can_parse_subroutine_call(debug);
    methodcall_test_can_parse_subroutine_call2(debug);

    stmt_test_assignment_statement_with_method_call(debug);
    stmt_test_assignment_statement_with_struct_access(debug);

    test_stmtblock_1(debug);

    if_test1(debug);
    if_test2(debug);

    whilestmt_test1(debug);
    whilestmt_test2(debug);
    
    trycatch_stmt_test(debug);
    
    for_test1(debug);

    return true;
}

bool test_suite_assignstmt(bool debug){
	
	status("test_suite_assignstmt");
	
	int count  = 7;
    int passed = 0;
    
    //7
    passed +=  assignstmt_test1(debug);
    passed +=  assignstmt_test_assign_char(debug);
    passed +=  assignstmt_test_assign_method_call_result(debug);
    passed +=  assignstmt_test_assign_method_call_result_2(debug);
    passed +=  assignstmt_test_assign_variable_with_array_index(debug);
    passed +=  assignstmt_test_can_assign_to_struct_member(debug);
    passed +=  assignstmt_test_type_declaration_for_variable(debug);

    return passed == count;
}

bool test_suite_method(bool debug) {
	
	status("test_suite_method");

    int count  = 4;
    int passed = 0;

    passed +=  method_test_can_parse_method_with_arguments(debug);
    passed +=  method_test_can_parse_method_without_arguments(debug);
    passed +=  method_test_can_parse_subroutine(debug);

    passed +=  declarg_test_parse_declared_argument(debug);

    return passed == count;
}

bool test_suite_struct(bool debug) {

	status("test_suite_struct");

    assert(1 == structmember_test_can_parse_struct_member(debug));

    return true;
}

bool test_suite_types(bool debug) {
	
	status("test_suite_types");

    int count  = 9;
    int passed = 0;

    passed +=  basictype_test_type_parsing_simple_type(debug);

    passed +=  simpletype_test_typenode_parsing(debug);
    passed +=  simpletype_test_typenode_parsing_anytype(debug);
    passed +=  simpletype_test_typenode_parsing_fails(debug);
    passed +=  simpletype_test_generic(debug);

    passed +=  subrtype_test_subroutine_type_parsing_subroutine_with_side_effects(debug);
    passed +=  subrtype_test_subroutine_type_parsing_subroutine_without_side_effects(debug);
    passed +=  subrtype_test_typename(debug);
    passed +=  subrtype_test_typename_subroutine_return_type(debug);

    structtype_test();
    structtype_test_type_param();
    structtype_test_generic();

    return passed == count;
}

bool test_suite_range(bool debug){
	
	status("test_suite_range");
	
	//test contains its own asserts,
	//so it will exit on failure
	range_test1(debug);
	
	return true;
}
