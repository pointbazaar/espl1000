#include <stdio.h>
#include <stdlib.h>

#include "statements/SwitchStmt.h"
#include "statements/CaseStmt.h"
#include "var/Variable.h"

#include "ast/util/free_ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

void parse_cases(struct SwitchStmt* s, struct TokenList* copy, bool debug);

struct SwitchStmt* makeSwitchStmt(struct TokenList* tokens, bool debug){

	if(debug){
		printf("SwitchStmt(...)\n");
	}

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, SWITCH)){
		
		printf("expected 'switch', but was: %s\n", list_code(copy, debug));
		exit(1);
		return NULL;
	}
	
	struct SwitchStmt* res = make(SwitchStmt);

	res->var = NULL;
	res->count_cases = 0;
	res->cases = malloc(sizeof(struct CaseStmt*)*1);

	res->var = makeVariable(copy,debug);
	if(res->var == NULL){
		free(res);
		return NULL;
	}

	if(!list_expect(copy, LCURLY)){
		
		printf("expected '{', but was: %s\n", list_code(copy, debug));
		exit(1);
		return NULL;
	}
	
	parse_cases(res, copy, debug);
	
	if(!list_expect(copy, RCURLY)){
		
		printf("expected '}', but was: %s\n", list_code(copy, debug));
		exit(1);
		return NULL;
	}
	
	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void parse_cases(struct SwitchStmt* s, struct TokenList* copy, bool debug){
	
	struct Token* head = list_head(copy);
	
	while(head->kind == CASE){
		
		struct CaseStmt* stmt = makeCaseStmt(copy, debug);
		if(stmt == NULL){
			printf("expected 'case Stmt', but was: \n");
			list_print(copy);
			exit(1);
		}
		s->cases[s->count_cases] = stmt;
		s->count_cases+=1;
		
		s->cases = realloc(s->cases, (s->count_cases+1)*(sizeof(struct CaseStmt*)));
		
		head = list_head(copy);
	}
}

