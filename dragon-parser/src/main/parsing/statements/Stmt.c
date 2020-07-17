#include <stdio.h>
#include <stdlib.h>

#include "Stmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/Token.h"
#include "WhileStmt.h"
#include "IfStmt.h"
#include "RetStmt.h"
#include "MethodCall.h"
#include "AssignStmt.h"

struct Stmt* makeStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Stmt(...)\n");
	}

	struct Stmt* res = malloc(sizeof(struct Stmt));

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
		if(res->m2 == NULL){
			//parsing is deterministic here. 
			//so this is a fatal error
			printf("expected while stmt, but was:\n");
			printf("%s\n", list_code(copy, debug));
			exit(1);
			return NULL;
		}

	} else if (first->kind == IF) {
		res->m3 = makeIfStmt		(copy,debug);
		if(res->m3 == NULL){
			//parsing is deterministic here. 
			//so this is a fatal error
			printf("expected if stmt, but was:\n");
			printf("%s\n", list_code(copy, debug));
			exit(1);
			return NULL;
		}

	} else if (first->kind == RETURN) {
		res->m4 = makeRetStmt		(copy,debug);
		if(res->m4 == NULL){
			//parsing is deterministic here. 
			//so this is a fatal error
			printf("expected return stmt, but was:\n");
			printf("%s\n", list_code(copy, debug));
			exit(1);
			return NULL;
		}

	} else {
		//TODO: we have to figure something out here.
		//i don't want 'let' statements
		//because it just messes up the code and is
		//just bloat to write.
		//but parsing should be straightforward
		//and mostly deterministic
		//because it gives good performance and
		//to give good error messages
		
		res->m5 = makeAssignStmt(copy,debug);
		
		if(res->m5 == NULL){
			
			res->m1 = makeMethodCall(copy,debug);
			if(res->m1 == NULL){
				printf("expected method call, but was:\n");
				printf("%s\n", list_code(copy, debug));
				exit(1);
			}
			if(!list_expect(copy, SEMICOLON)){
				exit(1);
			}
		}
	}
	
	if(debug){
		printf("sucess parsing Stmt\n");
	}

	list_set(tokens, copy);

	return res;
}

void freeStmt(struct Stmt* s){
	if(s->m1 != NULL){
		freeMethodCall(s->m1);
	}else if(s->m2 != NULL){
		freeWhileStmt(s->m2);
	}else if(s->m3 != NULL){
		freeIfStmt(s->m3);
	}else if(s->m4 != NULL){
		freeRetStmt(s->m4);
	}else if(s->m5 != NULL){
		freeAssignStmt(s->m5);
	}
	
	free(s);
}

