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
#include "astnodes/MethodCallTest.h"
#include "astnodes/RetStmtTest.h"
#include "astnodes/StmtTest.h"
#include "astnodes/SubrTypeTest.h"
#include "astnodes/WhileStmtTest.h"
#include "astnodes/BasicTypeWrappedTest.h"
#include "astnodes/DeclArgTest.h"
#include "astnodes/IfStmtTest.h"
#include "astnodes/MethodTest.h"
#include "astnodes/SimpleTypeTest.h"
#include "astnodes/StructDeclTest.h"
#include "astnodes/TermTest.h"

#include "commandline/TokenListTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void test_all(bool debug){
	
	bool res = test_all_inner(debug);
	if(res){
		printf("Passed all Tests.\n");
	}else{
		printf("Failed some Tests.\n");
	}
	
}

bool test_all_inner(bool debug){

	printf("running Dragon-Parser Tests...\n");

	//if some tests fail, the test suit should be 
	//stopped right there. no use running other tests
	//if one test already failed. we can work
	//on the failed test then.

	//execute all tests and give feedback
	//every test should stdout its name, such that failing tests can be identified easily

	//the tests should start with the easy tests first

	int count=0;
	int passed=0;

	int num_tests = 50;

	//https://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers
	//function pointer array for the tests
	int (*tests[num_tests]) (bool debug);

	tests[0] = test_tokenlist1;
	tests[1] = test_tokenlist_consume;

	tests[2] = boolconst_test_parse_bool_constant_node;

	tests[3] = charconst_test_parse_char_constant_node;
	tests[4] = charconst_test_parse_char_constant_node_newline;

	tests[5] = floatconst_test1;
	tests[6] = floatconst_test2;

	tests[7] = term_test_simple_term;
	tests[8] = term_test_variable_term;

	tests[9] = simplevar_test_parse_simple_indexed_variable;
	tests[10] = simplevar_test_parse_simple_variable;
	
	tests[11] = variable_test_parse_index_access;
	tests[12] = variable_test_parse_struct_member_access;
	tests[13] = variable_test_parse_struct_member_access_and_index_access;

	tests[14] = expr_recognize_2_op_expr;
	tests[15] = expr_test_simple_expression;
	tests[16] = expr_test_variable_name_expression;

	tests[17] = if_test1;
	tests[18] = if_test2;

	tests[19] = retstmt_test1;
	tests[20] = retstmt_test2;
	tests[21] = retstmt_test3;

	tests[22] = whilestmt_test1;

	tests[23] = assignstmt_test1;
	tests[24] = assignstmt_test_assign_char;
	tests[25] = assignstmt_test_assign_method_call_result;
	tests[26] = assignstmt_test_assign_method_call_result_2;
	tests[27] = assignstmt_test_assign_variable_with_array_index;
	tests[28] = assignstmt_test_can_assign_to_struct_member;
	tests[29] = assignstmt_test_type_declaration_for_variable;
	
	tests[30] = methodcall_test1;
	tests[31] = methodcall_test2;
	tests[32] = methodcall_test3;
	tests[33] = methodcall_test_can_parse_subroutine_call;
	tests[34] = methodcall_test_can_parse_subroutine_call2;
	
	tests[35] = stmt_test_assignment_statement_with_method_call;
	tests[36] = stmt_test_assignment_statement_with_struct_access;
	
	tests[37] = method_test_can_parse_method_with_arguments;
	tests[38] = method_test_can_parse_method_without_arguments;
	tests[39] = method_test_can_parse_subroutine;
	
	tests[40] = declarg_test_parse_declared_argument;
	
	tests[41] = structmember_test_can_parse_struct_member;
	
	tests[42] = basictypewrapped_test_type_parsing_simple_type;
	
	tests[43] = simpletype_test_typenode_parsing;
	tests[44] = simpletype_test_typenode_parsing_anytype;
	tests[45] = simpletype_test_typenode_parsing_fails;
	
	tests[46] = subrtype_test_subroutine_type_parsing_subroutine_with_side_effects;
	tests[47] = subrtype_test_subroutine_type_parsing_subroutine_without_side_effects;
	tests[48] = subrtype_test_typename;
	tests[49] = subrtype_test_typename_subroutine_return_type;
	
	for(int i=0; i<num_tests;i++){
		passed += tests[i](debug);
		count++;
		if(passed < count){
			printf("last test did not pass!\n");
			printf("Isolating the failing Test:\n");
			printf("------------------------------\n");
			tests[i](debug);
			break;
		}
	}	
	

	printf("passed %d of %d \n",passed,count);

	return passed == count;
}
