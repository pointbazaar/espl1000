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

#include <stdio.h>
#include <stdlib.h>
#include <stbool.h>

void test_all(bool debug){
	try{
		test_all_inner(debug);
	}catch(string e1){
		printf("catched! %s\n",(char*)e1.c_str());
	}catch(char const* e2){
		printf("catched! %s\n",e2);
	}
}

void test_all_inner(bool debug){

	printf("running Dragon-Parser Tests...\n");

	//execute all tests and give feedback
	//every test should stdout its name, such that failing tests can be identified easily

	//the tests should start with the easy tests first

	int count=0;
	int passed=0;


	printf("Test: BoolConst\n");
	passed += boolconst_test_parse_bool_constant_node(debug);

	printf("Test: CharConst\n");
	passed += charconst_test_parse_char_constant_node();
	passed += charconst_test_parse_char_constant_node_newline();
	count+=3;

	printf("Test: FloatConst\n");
	passed += floatconst_test1();
	passed += floatconst_test2();
	count+=2;

	printf("Test: Term\n");
	passed += term_test_simple_term(debug);
	passed += term_test_variable_term(debug);
	count+=2;

	
	printf("Test: SimpleVar\n");
	passed += simplevar_test_parse_simple_indexed_variable(debug);
	passed += simplevar_test_parse_simple_variable(debug);
	count += 2;

	printf("Test: Variable\n");
	passed += variable_test_parse_index_access(debug);
	passed += variable_test_parse_struct_member_access(debug);
	passed += variable_test_parse_struct_member_access_and_index_access(debug);
	count  += 3;


	printf("Test: Expr\n");
	passed += expr_recognize_2_op_expr(debug);
	passed += expr_test_simple_expression(debug);
	passed += expr_test_variable_name_expression(debug);
	count+=3;


	printf("Test: IfStmt\n");
	passed += if_test1(debug);
	passed += if_test2(debug);
	count += 2;

	printf("Test: RetStmt\n");
	passed += retstmt_test1(debug);
	passed += retstmt_test2(debug);
	passed += retstmt_test3(debug);
	count += 3;

	printf("Test: WhileStmt\n");
	passed += whilestmt_test1();
	count += 1;

	printf("Test: AssignStmt\n");
	passed += assignstmt_test1();
	passed += assignstmt_test_assign_char();
	passed += assignstmt_test_assign_method_call_result();
	passed += assignstmt_test_assign_method_call_result_2();
	passed += assignstmt_test_assign_variable_with_array_index();
	passed += assignstmt_test_can_assign_to_struct_member();
	passed += assignstmt_test_type_declaration_for_variable();
	count += 7;

	printf("Test: MethodCall\n");
	passed += methodcall_test1();
	passed += methodcall_test2();
	passed += methodcall_test3();
	passed += methodcall_test_can_parse_subroutine_call();
	passed += methodcall_test_can_parse_subroutine_call2();
	count += 6;

	printf("Test: Stmt\n");
	passed += stmt_test_assignment_statement_with_method_call();
	passed += stmt_test_assignment_statement_with_struct_access();
	count += 2;

	printf("Test: Method\n");
	passed += method_test_can_parse_method_with_arguments();
	passed += method_test_can_parse_method_without_arguments();
	passed += method_test_can_parse_subroutine();
	count += 3;


	printf("Test: DeclArg\n");
	passed += declarg_test_parse_declared_argument();
	count++;

	printf("Test: StructMember\n");
	passed += structmember_test_can_parse_struct_member();
	count++;

	


	printf("Test: BasicTypeWrapped\n");
	passed += basictypewrapped_test_type_parsing_simple_type();
	count+=1;

	printf("Test: SimpleType\n");
	passed += simpletype_test_typenode_parsing();
	passed += simpletype_test_typenode_parsing_anytype();
	passed += simpletype_test_typenode_parsing_fails();
	count+=3;

	printf("Test: SubrType\n");
	passed += subrtype_test_subroutine_type_parsing_subroutine_with_side_effects();
	passed += subrtype_test_subroutine_type_parsing_subroutine_without_side_effects();
	passed += subrtype_test_typename();
	passed += subrtype_test_typename_subroutine_return_type();
	count+=4;

	printf("passed %d of %d \n",passed,count);
}
