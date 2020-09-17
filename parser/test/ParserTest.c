#include "ParserTest.h"

#include "astnodes/BoolConstTest.h"
#include "astnodes/ExprTest.h"
#include "astnodes/NamespaceTest.h"
#include "astnodes/SimpleVarTest.h"
#include "astnodes/StructMemberTest.h"
#include "astnodes/VariableTest.h"
#include "astnodes/AssignStmtTest.h"
#include "astnodes/CharConstTest.h"
#include "astnodes/FloatConstTest.h"
#include "astnodes/LoopStmtTest.h"
#include "astnodes/MethodCallTest.h"
#include "astnodes/RetStmtTest.h"
#include "astnodes/BreakStmtTest.h"
#include "astnodes/StmtTest.h"
#include "astnodes/StmtBlockTest.h"
#include "astnodes/SubrTypeTest.h"
#include "astnodes/WhileStmtTest.h"
#include "astnodes/BasicTypeWrappedTest.h"
#include "astnodes/DeclArgTest.h"
#include "astnodes/IfStmtTest.h"
#include "astnodes/MethodTest.h"
#include "astnodes/SimpleTypeTest.h"
#include "astnodes/StructDeclTest.h"
#include "astnodes/TermTest.h"
#include "astnodes/RangeTest.h"
#include "astnodes/ForStmtTest.h"
#include "astnodes/SwitchStmtTest.h"
#include "astnodes/CaseStmtTest.h"

#include "commandline/TokenListTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FAILRET if(!passed){ printf("FAILED\n"); return false;}

//there are multiple test suites which test multiple aspects
//of the parser.

// --- START OF TEST SUITES ---

bool test_suite_tokenlist(bool debug);
bool test_suite_constnodes(bool debug);
bool test_suite_term_expr_simplevar_var(bool debug);

bool test_suite_stmts(bool debug);
bool test_suite_assignstmt(bool debug);

bool test_suite_method(bool debug);
bool test_suite_struct(bool debug);
bool test_suite_types(bool debug);
bool test_suite_range(bool debug);

// --- END OF TEST SUITES ---

int test_all(bool debug) {

    bool res = test_all_inner(debug);
    if(res) {
        printf("Passed all Tests.\n");
    } else {
        printf("Failed some Tests.\n");
    }
    return (res == true)?0:1;
}

bool test_all_inner(bool debug) {

    printf("running Dragon-Parser Tests...\n");

    //if some tests fail, the test suit should be
    //stopped right there. no use running other tests
    //if one test already failed. we can work
    //on the failed test then.

    //execute all tests and give feedback
    //every test should stdout its name, such that failing tests can be identified easily

    //the tests should start with the easy tests first


    if(!test_suite_tokenlist(debug)) {
        printf("suite_tokenlist failed\n");
        return false;
    }
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

    printf("Parser: passed all Test Suites\n");

    return true;
}

// --- TEST SUITE IMPLEMENTATIONS ---

bool test_suite_tokenlist(bool debug) {
	
	if(debug){ printf("test_suite_tokenlist\n"); }

    bool passed = true;

    passed &=  test_tokenlist1(debug);
    FAILRET
    passed &=  test_tokenlist_consume(debug);
    FAILRET

    passed &=  test_tokenlist_get(debug);
    FAILRET
    passed &=  test_tokenlist_startswith(debug);
    FAILRET

    passed &=  test_tokenlist_code(debug);
    FAILRET
    passed &=  test_tokenlist_stresstest(debug);
    FAILRET

    return passed;
}

bool test_suite_constnodes(bool debug) {
	
	if(debug){ printf("test_suite_constnodes\n"); }

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
	
	if(debug){ printf("test_suite_term_expr_simplevar_var\n"); }

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

	if(debug){ printf("test_suite_stmts\n"); }

    int count  = 17;
    int passed = 0;
    
    //1
    passed +=  loop_test1(debug);

    //3
    passed +=  retstmt_test1(debug);
    passed +=  retstmt_test2(debug);
    passed +=  retstmt_test3(debug);

    //5
    passed +=  methodcall_test1(debug);
    passed +=  methodcall_test2(debug);
    passed +=  methodcall_test3(debug);
    passed +=  methodcall_test_can_parse_subroutine_call(debug);
    passed +=  methodcall_test_can_parse_subroutine_call2(debug);

    //2
    passed +=  stmt_test_assignment_statement_with_method_call(debug);
    passed +=  stmt_test_assignment_statement_with_struct_access(debug);

    //1
    passed +=  test_stmtblock_1(debug);

    //2
    passed +=  if_test1(debug);
    passed +=  if_test2(debug);

    //2
    passed +=  whilestmt_test1(debug);
    passed +=  whilestmt_test2(debug);
    
    //1
    passed += break_test1(debug);
    
    //this test uses assert
    //so it needs no counting
    for_test1(debug);

    return passed == count;
}

bool test_suite_assignstmt(bool debug){
	
	if(debug){ printf("test_suite_assignstmt\n"); }
	
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
	
	if(debug){ printf("test_suite_method\n"); }

    int count  = 4;
    int passed = 0;

    passed +=  method_test_can_parse_method_with_arguments(debug);
    passed +=  method_test_can_parse_method_without_arguments(debug);
    passed +=  method_test_can_parse_subroutine(debug);

    passed +=  declarg_test_parse_declared_argument(debug);

    return passed == count;
}

bool test_suite_struct(bool debug) {

	if(debug){ printf("test_suite_struct\n"); }

    int count  = 1;
    int passed = 0;

    passed +=  structmember_test_can_parse_struct_member(debug);

    return passed == count;
}

bool test_suite_types(bool debug) {
	
	if(debug){ printf("test_suite_types\n"); }

    int count  = 8;
    int passed = 0;

    passed +=  basictypewrapped_test_type_parsing_simple_type(debug);

    passed +=  simpletype_test_typenode_parsing(debug);
    passed +=  simpletype_test_typenode_parsing_anytype(debug);
    passed +=  simpletype_test_typenode_parsing_fails(debug);

    passed +=  subrtype_test_subroutine_type_parsing_subroutine_with_side_effects(debug);
    passed +=  subrtype_test_subroutine_type_parsing_subroutine_without_side_effects(debug);
    passed +=  subrtype_test_typename(debug);
    passed +=  subrtype_test_typename_subroutine_return_type(debug);

    return passed == count;
}

bool test_suite_range(bool debug){
	
	if(debug){ printf("test_suite_range\n"); }
	
	//test contains its own asserts,
	//so it will exit on failure
	range_test1(debug);
	
	return true;
}
