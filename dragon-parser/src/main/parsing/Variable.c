#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Variable.h"
#include "SimpleVar.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/TokenList.h"

struct Variable* makeVariable(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Variable(...) from %s\n", list_code(tokens, debug));
	}

	struct Variable* res = malloc(sizeof(struct Variable));
	res->simpleVariableNode = NULL;

	res->memberAccessList = malloc(sizeof(struct Variable*)*1);
	res->count_memberAccessList = 0;

	struct TokenList* copy = list_copy(tokens);

	res->simpleVariableNode = makeSimpleVar(copy,debug);
	if(res->simpleVariableNode == NULL){return NULL;}

	if (list_size(copy) >= 2) {
		struct Token* next = list_get(copy, 0);
		while (next->kind == STRUCTMEMBERACCESS) {

			if(!list_expect(copy, STRUCTMEMBERACCESS)){return NULL;}

			struct Variable* myvar = makeVariable(copy,debug);
			if(myvar == NULL){return NULL;}

			res->memberAccessList[res->count_memberAccessList++] = myvar;
			res->memberAccessList = (struct Variable**)realloc(res->memberAccessList,sizeof(struct Variable*)*res->count_memberAccessList);

			if (list_size(copy) > 0) {
				next = list_get(copy,0);
				if(next == NULL){return NULL;}
			} else {
				break;
			}
		}
	}

	list_set(tokens,copy);

	return res;
}

