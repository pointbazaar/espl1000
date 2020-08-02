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
#include "../../../../../util/util.h"

struct Stmt* makeStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Stmt(...)\n");
	}
	
	if (list_size(tokens) == 0) { return NULL; }

	struct Stmt* res = smalloc(sizeof(struct Stmt));

	//init
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;

	struct TokenList* copy = list_copy(tokens);

	struct Token* first = list_head(copy);

	if (first->kind == LOOP) {
		//this->statementNode = LoopStatementNode(copy);
		printf("currently unsupported : 'LOOP'\n");
		
		freeTokenListShallow(copy);
		free(res);
		
		exit(1);
	} else if (first->kind == WHILE) {
		res->m2 = makeWhileStmt		(copy,debug);
		if(res->m2 == NULL){
			//parsing is deterministic here. 
			//so this is a fatal error
			printf("expected while stmt, but was:\n");
			list_print(copy);
			
			freeTokenListShallow(copy);
			free(res);
			
			exit(1);
		}

	} else if (first->kind == IF) {
		res->m3 = makeIfStmt		(copy,debug);
		if(res->m3 == NULL){
			//parsing is deterministic here. 
			//so this is a fatal error
			printf("expected if stmt, but was:\n");
			list_print(copy);
			
			freeTokenListShallow(copy);
			free(res);
			exit(1);
		}

	} else if (first->kind == RETURN) {
		res->m4 = makeRetStmt		(copy,debug);
		if(res->m4 == NULL){
			//parsing is deterministic here. 
			//so this is a fatal error
			printf("expected return stmt, but was:\n");
			list_print(copy);
			
			freeTokenListShallow(copy);
			free(res);
			exit(1);
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
				list_print(copy);
				
				freeTokenListShallow(copy);
				free(res);
				
				exit(1);
			}
			if(!list_expect(copy, SEMICOLON)){
				freeTokenListShallow(copy);
				free(res);
				
				exit(1);
			}
		}
	}
	
	if(debug){
		printf("sucess parsing Stmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

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

