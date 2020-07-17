#include <stdio.h>
#include <stdlib.h>

#include "WhileStmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/Token.h"
#include "../Expr.h"
#include "Stmt.h"

struct WhileStmt* makeWhileStmt(struct TokenList* tokens, bool debug){

	if(debug){
		printf("WhileStmt(...)\n");
	}

	struct WhileStmt* res = malloc(sizeof(struct WhileStmt));

	res->statements = malloc(sizeof(struct Stmt*)*1);
	res->count_statements = 0;

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, WHILE)){
		//this part can be parsed deterministically
		printf("expected 'while', but was: %s\n", list_code(copy, debug));
		exit(1);
		return NULL;
	}

	res->condition = makeExpr(copy,debug);
	if(res->condition == NULL){return NULL;}

	if(!list_expect(copy, LCURLY)){
		//this part can be parsed deterministically
		printf("expected '{', but was: %s\n", list_code(copy, debug));
		exit(1);
		return NULL;
	}

	struct Token* next = list_head(copy);
	if(next == NULL){return NULL;}

	while (next->kind != RCURLY) {

		struct Stmt* stmt = makeStmt(copy,debug);
		if(stmt == NULL){
			printf("expected a statement, but got: %s\n",list_code(copy, debug));
			exit(1);
		}
		
		res->statements[res->count_statements] = stmt;
		res->count_statements++;
		int newsize = res->count_statements;
		res->statements = realloc(res->statements, sizeof(struct Stmt*) * newsize);

		next = list_head(copy);
		if(next == NULL){return NULL;}
	}

	if(!list_expect(copy, RCURLY)){
		//this part can be parsed deterministically
		printf("expected '}', but was: %s\n", list_code(copy, debug));
		exit(1);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing WhileStmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeWhileStmt(struct WhileStmt* ws){
	freeExpr(ws->condition);
	for(int i=0;i < ws->count_statements; i++){
		freeStmt(ws->statements[i]);
	}
	free(ws);
}
