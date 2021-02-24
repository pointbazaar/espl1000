#include "AssignStmtTest.h"

#include "../../../main/commandline/TokenList.h"
#include "../../../main/commandline/TokenKeys.h"

#include "statements/AssignStmt.h"
#include "Expr.h"
#include "var/Variable.h"

#include "../../../../token/token.h"
#include "../../../../ast/free_ast.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

int assignstmt_test1(bool debug) {

	if(debug){
		printf("TEST: assignstmt_test1\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,debug);
	
	assert(a != NULL);
	
	freeAssignStmt(a);
	freeTokenList(tokens);

	return 1;
}

int assignstmt_test_assign_method_call_result(bool debug) {

	if(debug){
		printf("TEST: assignstmt_test_assign_method_call_result\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"nop"));

	list_add(tokens, makeToken(LPARENS));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(RPARENS));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,debug);
	
	assert(a != NULL);
	
	freeAssignStmt(a);
	freeTokenList(tokens);
	
	return 1;
}

int assignstmt_test_assign_method_call_result_2(bool debug) {

	if(debug){
		printf("TEST: assignstmt_test_assign_method_call_result_2\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"nop"));

	list_add(tokens, makeToken(LPARENS));
	list_add(tokens, makeToken(RPARENS));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,debug);
	assert(a != NULL);
	
	freeAssignStmt(a);
	freeTokenList(tokens);
	
	return 1;
}

int assignstmt_test_assign_variable_with_array_index(bool debug) {

	if(debug){
		printf("TEST: assignstmt_test_assign_variable_with_array_index\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"arr"));

	list_add(tokens, makeToken(LBRACKET));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(RBRACKET));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,debug);
	assert(a != NULL);
	
	freeAssignStmt(a);
	freeTokenList(tokens);
	
	return 1;
}

int assignstmt_test_assign_char(bool debug) {

	if(debug){
		printf("TEST: assignstmt_test_assign_char\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));

	list_add(tokens, makeToken2(CCONST,"x"));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,debug);
	
	assert(a != NULL);
	
	freeAssignStmt(a);
	freeTokenList(tokens);

	return 1;
}

int assignstmt_test_can_assign_to_struct_member(bool debug) {

	if(debug){
		printf("TEST: assignstmt_test_can_assign_to_struct_member\n");
	}

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken(STRUCTMEMBERACCESS));
	list_add(tokens, makeToken2(ID,"a"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));
	list_add(tokens, makeToken2(INTEGER,"3"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,debug);
	
	assert(a != NULL);
	
	freeAssignStmt(a);
	freeTokenList(tokens);
	
	return 1;
}

int assignstmt_test_type_declaration_for_variable(bool debug) {

	if(debug){
		printf("TEST: assignstmt_test_type_declaration_for_variable\n");
	}

	struct TokenList* tokens = makeTokenList();
	
	list_add(tokens, makeToken2(TYPEID,"Carrot"));
	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(ASSIGNOP,"="));
	list_add(tokens, makeToken2(INTEGER,"3"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,debug);
	if(a == NULL){return 0;}

	assert(a->optType != NULL);
	if(a->var == NULL){return 0;}
	
	assert(0 == a->var->count_memberAccessList);
	assert(0 == list_size(tokens));
	
	freeAssignStmt(a);
	freeTokenList(tokens);

	return 1;
}
