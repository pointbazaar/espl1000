#include "AssignStmtTest.hpp"

#include "../../main/commandline/TokenList.hpp"
#include "../../main/commandline/TokenKeys.hpp"
#include "../../main/commandline/Token.hpp"

#include "../../main/parsing/statements/AssignStmt.hpp"
#include "../../main/parsing/Expr.hpp"
#include "../../main/parsing/Variable.hpp"

int assignstmt_test1() {

	try {
		TokenList* tokens = new TokenList();

		tokens->add(Token(ID,"x"));
		tokens->add(Token(OPKEY,"="));
		tokens->add(Token(INTEGER,"4"));
		tokens->add(SEMICOLON);

		struct AssignStmt* a = makeAssignStmt(tokens,false);

		return 1;
	}catch (string e){
		return 0;
	}
}

int assignstmt_test_assign_method_call_result() {

	try {
		TokenList* tokens = new TokenList();

		tokens->add(Token(ID,"x"));
		tokens->add(Token(OPKEY,"="));

		//nop refers to the no operation method. it does nothing with its argument
		tokens->add(Token(ID,"nop"));

		tokens->add(LPARENS);
		tokens->add(Token(INTEGER,"4"));
		tokens->add(RPARENS);

		tokens->add(SEMICOLON);

		struct AssignStmt* a = makeAssignStmt(tokens,false);
		return 1;
	}catch (string e){
		return 0;
	}
}

int assignstmt_test_assign_method_call_result_2() {

	try {
		TokenList* tokens = new TokenList();

		tokens->add(Token(ID,"x"));
		tokens->add(Token(OPKEY,"="));

		//nop refers to the no operation method. it does nothing with its argument
		tokens->add(Token(ID,"nop"));

		tokens->add(LPARENS);
		tokens->add(RPARENS);

		tokens->add(SEMICOLON);

		struct AssignStmt* a = makeAssignStmt(tokens,false);
		return 1;
	}catch (string e){
		return 0;
	}
}

int assignstmt_test_assign_variable_with_array_index() {

	try {
		TokenList* tokens = new TokenList();

		tokens->add(Token(ID,"x"));
		tokens->add(Token(OPKEY,"="));

		//nop refers to the no operation method. it does nothing with its argument
		tokens->add(Token(ID,"arr"));

		tokens->add(LBRACKET);
		tokens->add(Token(INTEGER,"4"));
		tokens->add(RBRACKET);

		tokens->add(SEMICOLON);

		struct AssignStmt* a = makeAssignStmt(tokens,false);
		return 1;
	}catch (string e){
		return 0;
	}
}

int assignstmt_test_assign_char() {

	try {
		TokenList* tokens = new TokenList();

		tokens->add(Token(ID,"x"));
		tokens->add(Token(OPKEY,"="));

		tokens->add(Token(CCONST,"x"));

		tokens->add(SEMICOLON);

		struct AssignStmt* a = makeAssignStmt(tokens,false);

		return 1;
	}catch (string e){
		return 0;
	}
}

int assignstmt_test_can_assign_to_struct_member() {

	try {
		TokenList* tokens = new TokenList();
		tokens->add(Token(ID,"x"));
		tokens->add(STRUCTMEMBERACCESS);
		tokens->add(Token(ID,"a"));
		tokens->add(Token(OPKEY,"="));
		tokens->add(Token(INTEGER,"3"));
		tokens->add(SEMICOLON);

		struct AssignStmt* a = makeAssignStmt(tokens,false);

		return 1;
	}catch (string e){
		return 0;
	}
}

int assignstmt_test_type_declaration_for_variable() {

	TokenList* tokens = new TokenList();
	tokens->add(Token(TYPEIDENTIFIER,"PInt"));
	tokens->add(Token(ID,"x"));
	tokens->add(Token(OPKEY,"="));
	tokens->add(Token(INTEGER,"3"));
	tokens->add(SEMICOLON);

	struct AssignStmt* a = makeAssignStmt(tokens,false);
	bool assert1 = (a->optTypeNode != NULL);
	bool assert2 = (0 == a->variableNode->count_memberAccessList);
	bool assert3 = (0 == tokens->size());

	return (assert1&&assert2&&assert3)?1:0;
}
