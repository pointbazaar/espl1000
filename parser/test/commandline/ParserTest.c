#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "../astnodes/const/ConstValueTest.h"

#include "../astnodes/expr/ExprTest.h"
#include "../astnodes/expr/UnOpTermTest.h"
#include "../astnodes/expr/TermTest.h"

#include "../astnodes/RangeTest.h"

#include "../astnodes/NamespaceTest.h"

#include "../astnodes/var/SimpleVarTest.h"
#include "../astnodes/var/VariableTest.h"

#include "../astnodes/StmtBlockTest.h"

#include "../astnodes/types/BasicTypeTest.h"
#include "../astnodes/types/SubrTypeTest.h"
#include "../astnodes/types/SimpleTypeTest.h"
#include "../astnodes/types/StructTypeTest.h"

#include "../astnodes/subr/DeclArgTest.h"
#include "../astnodes/subr/MethodTest.h"

#include "../astnodes/struct/StructDeclTest.h"
#include "../astnodes/struct/StructMemberTest.h"

#include "../astnodes/statements/ForStmtTest.h"
#include "../astnodes/statements/IfStmtTest.h"
#include "../astnodes/statements/WhileStmtTest.h"
#include "../astnodes/statements/CallTest.h"
#include "../astnodes/statements/RetStmtTest.h"
#include "../astnodes/statements/StmtTest.h"
#include "../astnodes/statements/AssignStmtTest.h"

#include "parser/test/astnodes/statements/MAssignStmtTest.h"
#include "parser/test/astnodes/expr/MDirectTest.h"

#include "test.h"

void test_parser_suite_unop() {

	test_unop_without();
	test_unop_with();
}

void test_parser_suite_namespace() {

	namespace_test_can_parse_namespace_with_1_empty_struct();
	namespace_test_can_parse_namespace_with_1_empty_method();
}

void test_parser_suite_constnodes() {

	test_boolconst_parse_bool_constant_node();
	test_charconst_parse_char_constant_node();
	test_charconst_parse_char_constant_node_newline();
}

void test_parser_suite_term() {

	term_test_simple_term();
	term_test_variable_term();
	term_test_parentheses();
	term_test_mdirect();

	mdirect_test_1();
}

void test_parser_suite_expr() {

	expr_recognize_2_op_expr();
	expr_test_simple_expression();
	expr_test_variable_name_expression();

	expr_test_comparison();

	expr_test_3_terms();
}

void test_parser_suite_simplevar() {

	simplevar_test_parse_simple_indexed_variable();
	simplevar_test_parse_simple_variable();
	simplevar_test_2_indices();
}

void test_parser_suite_var() {

	variable_test_parse_index_access();
	variable_test_parse_n_index_access();
	variable_test_parse_struct_member_access();
	variable_test_parse_struct_member_access_and_index_access();
}

void test_parser_suite_stmts() {

	retstmt_test1();
	retstmt_test2();
	retstmt_test3();

	methodcall_test1();
	methodcall_test2();
	methodcall_test3();
	methodcall_test_can_parse_subroutine_call();
	methodcall_test_can_parse_subroutine_call2();
	methodcall_test_can_parse_struct_member_access();
	methodcall_test_can_parse_array_access();

	stmt_test_assignment_statement_with_method_call();
	stmt_test_assignment_statement_with_struct_access();

	test_stmtblock_1();

	if_test1();
	if_test2();

	whilestmt_test1();
	whilestmt_test2();

	for_test1();

	massignstmt_test1();
}

void test_parser_suite_assignstmt() {

	assignstmt_test1();
	assignstmt_test_assign_char();
	assignstmt_test_assign_method_call_result();
	assignstmt_test_assign_method_call_result_2();
	assignstmt_test_assign_variable_with_array_index();
	assignstmt_test_can_assign_to_struct_member();
	assignstmt_test_type_declaration_for_variable();
}

void test_parser_suite_method() {

	method_test_can_parse_method_with_arguments();
	method_test_can_parse_method_without_arguments();
	method_test_can_parse_subroutine();

	declarg_test_parse_declared_argument();
}

void test_parser_suite_struct() {

	structdecl_test_can_parse_empty_struct_decl();
	structdecl_test_can_parse_struct_with_1_member();
	structdecl_test_can_parse_struct_with_2_members();

	structmember_test_can_parse_struct_member();
}

void test_parser_suite_types() {

	basictype_test_type_parsing_simple_type();

	simpletype_test_typenode_parsing();
	simpletype_test_typenode_parsing_anytype();
	simpletype_test_typenode_parsing_fails();

	subrtype_test_subroutine_type_parsing_subroutine_with_side_effects();
	subrtype_test_subroutine_type_parsing_subroutine_without_side_effects();
	subrtype_test_typename();
	subrtype_test_typename_subroutine_return_type();

	structtype_test();
}

void test_parser_suite_range() {

	range_test1();
}
