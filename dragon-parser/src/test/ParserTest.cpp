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

void test_all(){

	cout << "running Dragon-Parser Tests..." << endl;

	//TODO: execute all tests and give feedback
	//every test should stdout its name, such that failing tests can be identified easily

	int count=0;
	int passed=0;

	//nonterminals
		//statements
			//controlflow
			passed += if_test1();
			passed += if_test2();
			count += 2;

			passed += retstmt_test1();
			passed += retstmt_test2();
			passed += retstmt_test3();
			count += 3;

			passed += whilestmt_test1();
			count += 1;

		passed += assignstmt_test1();
		passed += assignstmt_test_assign_char();
		passed += assignstmt_test_assign_method_call_result();
		passed += assignstmt_test_assign_method_call_result_2();
		passed += assignstmt_test_assign_variable_with_array_index();
		passed += assignstmt_test_can_assign_to_struct_member();
		passed += assignstmt_test_type_declaration_for_variable();
		count += 7;

		passed += methodcall_test1();
		passed += methodcall_test2();
		passed += methodcall_test3();
		passed += methodcall_test_can_parse_subroutine_call();
		passed += methodcall_test_can_parse_subroutine_call2();
		count += 6;

		passed += stmt_test_assignment_statement_with_method_call();
		passed += stmt_test_assignment_statement_with_struct_access();
		count += 2;

		//upperscopes
		passed += method_test_can_parse_method_with_arguments();
		passed += method_test_can_parse_method_without_arguments();
		passed += method_test_can_parse_subroutine();
		count += 3;


	passed += declarg_test_parse_declared_argument();
	count++;

	passed += structmember_test_can_parse_struct_member();
	count++;

	passed += term_test_simple_term();
	passed += term_test_variable_term();
	count+=2;

	passed += variable_test_parse_index_access();
	passed += variable_test_parse_struct_member_access();
	passed += variable_test_parse_struct_member_access_and_index_access();
	count  += 3;

	//terminal
	passed += boolconst_test_parse_bool_constant_node();
	passed += charconst_test_parse_char_constant_node();
	passed += charconst_test_parse_char_constant_node_newline();
	count+=3;

	passed += expr_recognize_string_constant_expression();
	passed += expr_test_simple_expression();
	passed += expr_test_variable_name_expression();
	count+=3;

	passed += floatconst_test1();
	passed += floatconst_test2();
	count+=2;

	passed += simplevar_test_parse_simple_indexed_variable();
	passed += simplevar_test_parse_simple_variable();
	count += 2;

	//typenodes
	passed += basictypewrapped_test_type_parsing_simple_type();
	count+=1;

	passed += simpletype_test_typenode_parsing();
	passed += simpletype_test_typenode_parsing_anytype();
	passed += simpletype_test_typenode_parsing_fails();
	count+=3;

	passed += subrtype_test_subroutine_type_parsing_subroutine_with_side_effects();
	passed += subrtype_test_subroutine_type_parsing_subroutine_without_side_effects();
	passed += subrtype_test_typename();
	passed += subrtype_test_typename_subroutine_return_type();
	count+=4;

	cout << "passed " << passed << "of " << count << endl;;
}
