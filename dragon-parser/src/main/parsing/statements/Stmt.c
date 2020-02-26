#include <stdio.h>
#include <stdlib.h>

#include "Stmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/Token.h"
#include "../../commandline/smalloc.h"
#include "WhileStmt.h"
#include "IfStmt.h"
#include "RetStmt.h"
#include "MethodCall.h"
#include "AssignStmt.h"

struct Stmt* makeStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Stmt(...)\n");
	}

	struct Stmt* res = smalloc(sizeof(struct Stmt));

	//init
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;

	struct TokenList* copy = list_copy(tokens);

	if (list_size(copy) == 0) {
		//("tried to parse a Statement, but there are no tokens left");
		return NULL;
	}

	struct Token* first = list_get(copy, 0);
	if(first == NULL){return NULL;}

	if (first->kind == LOOP) {
		//this->statementNode = LoopStatementNode(copy);
		printf("currently unsupported : 'LOOP'\n");
		exit(1);
	} else if (first->kind == WHILE) {
		res->m2 = makeWhileStmt		(copy,debug);
		if(res->m2 == NULL){return NULL;}

	} else if (first->kind == IF) {
		res->m3 = makeIfStmt		(copy,debug);
		if(res->m3 == NULL){return NULL;}

	} else if (first->kind == RETURN) {
		res->m4 = makeRetStmt		(copy,debug);
		if(res->m4 == NULL){return NULL;}

	} else {
		//TODO: we have to figure something out here.
		//i don't want 'let' statements
		//because it just messes up the code and is
		//just bloat to write.
		//but parsing should be straightforward. to give good error messages

		bool fail = false;

		struct TokenList* copy2 = list_copy(copy);
		res->m1 = makeMethodCall(copy2,debug);
		if(res->m1 == NULL){fail = true;}

		if(!list_expect(copy2, SEMICOLON)){fail = true;}

		if(!fail){
			list_set(copy, copy2);
		}else {
			res->m5 = makeAssignStmt(copy,debug);
		}
	}

	list_set(tokens, copy);

	return res;
}

