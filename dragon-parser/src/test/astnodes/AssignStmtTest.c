#include "AssignStmtTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/statements/AssignStmt.h"
#include "../../main/parsing/Expr.h"
#include "../../main/parsing/Variable.h"

#include <stdio.h>
#include <stdbool.h>

int assignstmt_test1() {

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(OPKEY,"="));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,false);

	return (a==NULL)?0:1;
}

int assignstmt_test_assign_method_call_result() {

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(OPKEY,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"nop"));

	list_add(tokens, makeToken(LPARENS));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(RPARENS));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,false);
	return (a==NULL)?0:1;
		
}

int assignstmt_test_assign_method_call_result_2() {

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(OPKEY,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"nop"));

	list_add(tokens, makeToken(LPARENS));
	list_add(tokens, makeToken(RPARENS));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,false);
	return (a==NULL)?0:1;
		
}

int assignstmt_test_assign_variable_with_array_index() {

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(OPKEY,"="));

	//nop refers to the no operation method. it does nothing with its argument
	list_add(tokens, makeToken2(ID,"arr"));

	list_add(tokens, makeToken(LBRACKET));
	list_add(tokens, makeToken2(INTEGER,"4"));
	list_add(tokens, makeToken(RBRACKET));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,false);
	return (a==NULL)?0:1;

}

int assignstmt_test_assign_char() {

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(OPKEY,"="));

	list_add(tokens, makeToken2(CCONST,"x"));

	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,false);

	return (a==NULL)?0:1;

}

int assignstmt_test_can_assign_to_struct_member() {

	struct TokenList* tokens = makeTokenList();

	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken(STRUCTMEMBERACCESS));
	list_add(tokens, makeToken2(ID,"a"));
	list_add(tokens, makeToken2(OPKEY,"="));
	list_add(tokens, makeToken2(INTEGER,"3"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,false);
	return (a==NULL)?0:1;

}

int assignstmt_test_type_declaration_for_variable() {

	struct TokenList* tokens = makeTokenList();
	list_add(tokens, makeToken2(TYPEIDENTIFIER,"PInt"));
	list_add(tokens, makeToken2(ID,"x"));
	list_add(tokens, makeToken2(OPKEY,"="));
	list_add(tokens, makeToken2(INTEGER,"3"));
	list_add(tokens, makeToken(SEMICOLON));

	struct AssignStmt* a = makeAssignStmt(tokens,false);
	bool assert1 = (a->optTypeNode != NULL);
	bool assert2 = (0 == a->variableNode->count_memberAccessList);
	bool assert3 = (0 == list_size(tokens));

	return (assert1&&assert2&&assert3)?1:0;
}
