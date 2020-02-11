#include "ParserTest.hpp"
       
#include "astnodes/BoolConstTest.hpp"  
#include "astnodes/ExprTest.hpp"   
#include "astnodes/NamespaceTest.hpp"   
#include "astnodes/SimpleVarTest.hpp"   
#include "astnodes/StructMemberTest.hpp"  
#include "astnodes/VariableTest.hpp"
#include "astnodes/AssignStmtTest.hpp"        
#include "astnodes/CharConstTest.hpp"  
#include "astnodes/FloatConstTest.hpp"  
#include "astnodes/MethodCallTest.hpp"  
#include "astnodes/RetStmtTest.hpp"     
#include "astnodes/StmtTest.hpp"        
#include "astnodes/SubrTypeTest.hpp"      
#include "astnodes/WhileStmtTest.hpp"
#include "astnodes/BasicTypeWrappedTest.hpp"  
#include "astnodes/DeclArgTest.hpp"    
#include "astnodes/IfStmtTest.hpp"      
#include "astnodes/MethodTest.hpp"      
#include "astnodes/SimpleTypeTest.hpp"  
#include "astnodes/StructDeclTest.hpp"  
#include "astnodes/TermTest.hpp"

#include <iostream>

using namespace std;

void test_all(bool debug){
	try{
		test_all_inner(debug);
	}catch(string e1){
		cout << "catched!" << endl;
		cout << e1 << endl;
	}catch(char const* e2){
		cout << "catched!" << endl;
		cout << e2 << endl;
	}
}

void test_all_inner(bool debug){

	cout << "running Dragon-Parser Tests..." << endl;

	//execute all tests and give feedback
	//every test should stdout its name, such that failing tests can be identified easily

	//the tests should start with the easy tests first

	int count=0;
	int passed=0;


	cout << "Test: BoolConst" << endl;
	passed += boolconst_test_parse_bool_constant_node();

	cout << "Test: CharConst" << endl;
	passed += charconst_test_parse_char_constant_node();
	passed += charconst_test_parse_char_constant_node_newline();
	count+=3;

	cout << "Test: FloatConst" << endl;
	passed += floatconst_test1();
	passed += floatconst_test2();
	count+=2;

	cout << "Test: Term" << endl;
	passed += term_test_simple_term();
	passed += term_test_variable_term();
	count+=2;

	
	cout << "Test: SimpleVar" << endl;
	passed += simplevar_test_parse_simple_indexed_variable(debug);
	passed += simplevar_test_parse_simple_variable(debug);
	count += 2;

	cout << "Test: Variable" << endl;
	passed += variable_test_parse_index_access(debug);
	passed += variable_test_parse_struct_member_access(debug);
	passed += variable_test_parse_struct_member_access_and_index_access(debug);
	count  += 3;


	cout << "Test: Expr" << endl;
	passed += expr_recognize_string_constant_expression();
	passed += expr_test_simple_expression();
	passed += expr_test_variable_name_expression();
	count+=3;


	cout << "Test: IfStmt" << endl;
	passed += if_test1();
	passed += if_test2();
	count += 2;

	cout << "Test: RetStmt" << endl;
	passed += retstmt_test1();
	passed += retstmt_test2();
	passed += retstmt_test3();
	count += 3;

	cout << "Test: WhileStmt" << endl;
	passed += whilestmt_test1();
	count += 1;

	cout << "Test: AssignStmt" << endl;
	passed += assignstmt_test1();
	passed += assignstmt_test_assign_char();
	passed += assignstmt_test_assign_method_call_result();
	passed += assignstmt_test_assign_method_call_result_2();
	passed += assignstmt_test_assign_variable_with_array_index();
	passed += assignstmt_test_can_assign_to_struct_member();
	passed += assignstmt_test_type_declaration_for_variable();
	count += 7;

	cout << "Test: MethodCall" << endl;
	passed += methodcall_test1();
	passed += methodcall_test2();
	passed += methodcall_test3();
	passed += methodcall_test_can_parse_subroutine_call();
	passed += methodcall_test_can_parse_subroutine_call2();
	count += 6;

	cout << "Test: Stmt" << endl;
	passed += stmt_test_assignment_statement_with_method_call();
	passed += stmt_test_assignment_statement_with_struct_access();
	count += 2;

	cout << "Test: Method" << endl;
	passed += method_test_can_parse_method_with_arguments();
	passed += method_test_can_parse_method_without_arguments();
	passed += method_test_can_parse_subroutine();
	count += 3;


	cout << "Test: DeclArg" << endl;
	passed += declarg_test_parse_declared_argument();
	count++;

	cout << "Test: StructMember" << endl;
	passed += structmember_test_can_parse_struct_member();
	count++;

	


	cout << "Test: BasicTypeWrapped" << endl;
	passed += basictypewrapped_test_type_parsing_simple_type();
	count+=1;

	cout << "Test: SimpleType" << endl;
	passed += simpletype_test_typenode_parsing();
	passed += simpletype_test_typenode_parsing_anytype();
	passed += simpletype_test_typenode_parsing_fails();
	count+=3;

	cout << "Test: SubrType" << endl;
	passed += subrtype_test_subroutine_type_parsing_subroutine_with_side_effects();
	passed += subrtype_test_subroutine_type_parsing_subroutine_without_side_effects();
	passed += subrtype_test_typename();
	passed += subrtype_test_typename_subroutine_return_type();
	count+=4;

	cout << "passed " << passed << "of " << count << endl;;
}
