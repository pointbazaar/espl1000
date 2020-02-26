#include <stdio.h>
#include <stdlib.h>

#include "WhileStmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/Token.h"
#include "../../commandline/smalloc.h"
#include "../Expr.h"
#include "Stmt.h"

struct WhileStmt* makeWhileStmt(struct TokenList* tokens, bool debug){

	if(debug){
		printf("WhileStmt(...)\n");
	}

	struct WhileStmt* res = smalloc(sizeof(struct WhileStmt));

	res->statements = smalloc(sizeof(struct Stmt*)*1);
	res->count_statements = 0;

	struct TokenList* copy = makeTokenList(tokens);

	if(!list_expect(copy, WHILE)){return NULL;}

	res->condition = makeExpr(copy,debug);
	if(res->condition == NULL){return NULL;}

	if(!list_expect(copy, LCURLY)){return NULL;}

	struct Token* next = list_get(copy, 0);
	if(next == NULL){return NULL;}

	while (next->kind != RCURLY) {

		res->statements[res->count_statements] = makeStmt(&copy,debug);
		res->count_statements++;
		int newsize = res->count_statements;
		res->statements = realloc(res->statements, sizeof(struct Stmt*) * newsize);

		next = list_get(copy, 0);
		if(next == NULL){return NULL;}
	}

	if(!list_expect(copy, RCURLY)){return NULL;}

	list_set(tokens, copy);

	return res;
}

