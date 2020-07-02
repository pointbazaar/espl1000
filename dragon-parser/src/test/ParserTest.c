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

	int num_tests_max = 55;

	//https://stackoverflow.com/questions/252748/how-can-i-use-an-array-of-function-pointers
	//function pointer array for the tests
	int (*tests[num_tests_max]) (bool debug);

	int k = 0; //our index counter

	tests[k++] = test_tokenlist1;
	tests[k++] = test_tokenlist_consume;
	tests[k++] = test_tokenlist_get;
	tests[k++] = test_tokenlist_startswith;

	tests[k++] = boolconst_test_parse_bool_constant_node;

	tests[k++] = charconst_test_parse_char_constant_node;
	tests[k++] = charconst_test_parse_char_constant_node_newline;

	tests[k++] = floatconst_test1;
	tests[k++] = floatconst_test2;

	tests[k++] = term_test_simple_term;
	tests[k++] = term_test_variable_term;

	tests[k++] = simplevar_test_parse_simple_indexed_variable;
	tests[k++] = simplevar_test_parse_simple_variable;
	
	tests[k++] = variable_test_parse_index_access;
	tests[k++] = variable_test_parse_struct_member_access;
	tests[k++] = variable_test_parse_struct_member_access_and_index_access;

	tests[k++] = expr_recognize_2_op_expr;
	tests[k++] = expr_test_simple_expression;
	tests[k++] = expr_test_variable_name_expression;

	tests[k++] = if_test1;
	tests[k++] = if_test2;

	tests[k++] = retstmt_test1;
	tests[k++] = retstmt_test2;
	tests[k++] = retstmt_test3;

	tests[k++] = whilestmt_test1;

	tests[k++] = assignstmt_test1;
	tests[k++] = assignstmt_test_assign_char;
	tests[k++] = assignstmt_test_assign_method_call_result;
	tests[k++] = assignstmt_test_assign_method_call_result_2;
	tests[k++] = assignstmt_test_assign_variable_with_array_index;
	tests[k++] = assignstmt_test_can_assign_to_struct_member;
	tests[k++] = assignstmt_test_type_declaration_for_variable;
	
	tests[k++] = methodcall_test1;
	tests[k++] = methodcall_test2;
	tests[k++] = methodcall_test3;
	tests[k++] = methodcall_test_can_parse_subroutine_call;
	tests[k++] = methodcall_test_can_parse_subroutine_call2;
	
	tests[k++] = stmt_test_assignment_statement_with_method_call;
	tests[k++] = stmt_test_assignment_statement_with_struct_access;
	
	tests[k++] = method_test_can_parse_method_with_arguments;
	tests[k++] = method_test_can_parse_method_without_arguments;
	tests[k++] = method_test_can_parse_subroutine;
	
	tests[k++] = declarg_test_parse_declared_argument;
	
	tests[k++] = structmember_test_can_parse_struct_member;
	
	tests[k++] = basictypewrapped_test_type_parsing_simple_type;
	
	tests[k++] = simpletype_test_typenode_parsing;
	tests[k++] = simpletype_test_typenode_parsing_anytype;
	tests[k++] = simpletype_test_typenode_parsing_fails;
	
	tests[k++] = subrtype_test_subroutine_type_parsing_subroutine_with_side_effects;
	tests[k++] = subrtype_test_subroutine_type_parsing_subroutine_without_side_effects;
	tests[k++] = subrtype_test_typename;
	tests[k++] = subrtype_test_typename_subroutine_return_type;
	
	for(int i=0; i < k;i++){
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
