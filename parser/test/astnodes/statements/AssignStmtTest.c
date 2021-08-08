#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <parser/test/test_parser_util.h>

#include "AssignStmtTest.h"

#include "statements/AssignStmt.h"
#include "expr/Expr.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

int assignstmt_test1() {

	status_test("assignstmt_test1");

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens);
	
	assert(a != NULL);

	free_assign_stmt(a);
	freeTokenList(tokens);

	return 1;
}

int assignstmt_test_assign_method_call_result() {

	status_test("assignstmt_test_assign_method_call_result");

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"nop"));

	list_add(tokens, makeToken(LPARENS));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(RPARENS));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens);
	
	assert(a != NULL);

	free_assign_stmt(a);
	freeTokenList(tokens);
	
	return 1;
}

int assignstmt_test_assign_method_call_result_2() {

	status_test("assignstmt_test_assign_method_call_result_2");

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"nop"));

	list_add(tokens, makeToken(LPARENS));
	list_add(tokens, makeToken(RPARENS));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens);
	assert(a != NULL);

	free_assign_stmt(a);
	freeTokenList(tokens);
	
	return 1;
}

int assignstmt_test_assign_variable_with_array_index() {

	status_test("assignstmt_test_assign_variable_with_array_index");

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"arr"));

	list_add(tokens, makeToken(LBRACKET));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(RBRACKET));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens);
	assert(a != NULL);

	free_assign_stmt(a);
	freeTokenList(tokens);
	
	return 1;
}

int assignstmt_test_assign_char() {

	status_test("assignstmt_test_assign_char");

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));

	list_add(tokens, makeToken2(CCONST,"x"));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens);
	
	assert(a != NULL);

	free_assign_stmt(a);
	freeTokenList(tokens);

	return 1;
}

int assignstmt_test_can_assign_to_struct_member() {

	status_test("assignstmt_test_can_assign_to_struct_member");

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken(STRUCTMEMBERACCESS));
	list_add(tokens, makeToken2(ID,"a"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));
	list_add(tokens, makeToken2(INTEGER,"3"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens);
	
	assert(a != NULL);

	free_assign_stmt(a);
	freeTokenList(tokens);
	
	return 1;
}

int assignstmt_test_type_declaration_for_variable() {

	status_test("assignstmt_test_type_declaration_for_variable");

	struct TokenList* tokens = makeTokenList();
	
	list_add(tokens, makeToken2(TYPEID,"Carrot"));
	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));
	list_add(tokens, makeToken2(INTEGER,"3"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens);
	if(a == NULL){return 0;}

	assert(a->opt_type != NULL);
	if(a->var == NULL){return 0;}
	
	assert(NULL == a->var->member_access);
	assert(0 == list_size(tokens));

	free_assign_stmt(a);
	freeTokenList(tokens);

	return 1;
}
