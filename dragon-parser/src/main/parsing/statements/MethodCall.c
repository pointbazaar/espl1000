#include <stdio.h>
#include <stdlib.h>

#include "MethodCall.h"
#include "../Identifier.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../Expr.h"


struct MethodCall* makeMethodCall(struct TokenList* tokens,bool debug) {

	if(debug){
		printf("MethodCall(...) from: %s\n", list_code(tokens, debug));
	}

	struct MethodCall* res = malloc(sizeof(struct MethodCall));
	if(res==NULL){return NULL;}

	res->args = malloc(sizeof(struct Expr*)*1);

	res->count_args = 0;

	struct TokenList* copy = list_copy(tokens);

	struct Identifier* id = makeIdentifier(copy,debug);
	if(id == NULL){return NULL;}

	res->methodName = id->identifier;

	if(debug){
		printf("try to parse LPARENS\n");
	}

	if(list_size(copy) == 0){return NULL;}
	if(!list_expect(copy, LPARENS)){return NULL;}

	if(debug){
		printf("try to parse args\n");
	}

	if(list_size(copy) == 0){return NULL;}
	struct Token* next = list_head(copy);

	bool found = false;
	while (next->kind != RPARENS) {
		if (found) {
			if(!list_expect(copy, COMMA)){return NULL;}
		}

		struct Expr* expr = makeExpr(copy,debug);
		if(expr == NULL){return NULL;}

		res->args[res->count_args] = expr;
		res->count_args += 1;

		res->args = realloc(res->args, sizeof(struct Expr*) * (res->count_args + 1));

		next = list_head(copy);
		if(next == NULL){return NULL;}

		found = true;
	}

	if(!list_expect(copy, RPARENS)){return NULL;}
	
	if(debug){
		printf("sucess parsing MethodCall\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeMethodCall(struct MethodCall* mc){
	printf("DEBUG: freeMethodCall\n");
	//TODO: inline mc->methodName
	for(int i=0;i < mc->count_args; i++){
		freeExpr(mc->args[i]);
	}
	printf("DEBUG: freeMethodCall 2\n");
	free(mc);
}
