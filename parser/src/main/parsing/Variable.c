#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Variable.h"
#include "SimpleVar.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/TokenList.h"
#include "../../../../util/util.h"

struct Variable* makeVariable(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Variable(...) from ");
		list_print(tokens);
	}
	
	if(list_size(tokens) < 1){ return NULL; }

	struct Variable* res = smalloc(sizeof(struct Variable));
	res->simpleVar = NULL;

	res->memberAccessList = smalloc(sizeof(struct Variable*)*1);
	res->count_memberAccessList = 0;

	struct TokenList* copy = list_copy(tokens);

	res->simpleVar = makeSimpleVar(copy,debug);
	if(res->simpleVar == NULL){
		freeTokenListShallow(copy);
		free(res->memberAccessList);
		free(res);
		return NULL;
	}

	if (list_size(copy) >= 2) {
		struct Token* next = list_head(copy);
		while (next->kind == STRUCTMEMBERACCESS) {

			if(!list_expect(copy, STRUCTMEMBERACCESS)){return NULL;}

			struct Variable* myvar = makeVariable(copy,debug);
			if(myvar == NULL){
				free(res->memberAccessList);
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}

			res->memberAccessList[res->count_memberAccessList] = myvar;
			res->count_memberAccessList += 1;
			size_t newsize = sizeof(struct Variable*)*(res->count_memberAccessList+1);
			res->memberAccessList = (struct Variable**)realloc(res->memberAccessList, newsize);

			if (list_size(copy) > 0) {
				next = list_head(copy);
				if(next == NULL){
					free(res->memberAccessList);
					free(res);
					freeTokenListShallow(copy);
					return NULL;
				}
			} else {
				break;
			}
		}
	}
	
	if(debug){
		printf("sucess parsing Variable\n");
	}

	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}

void freeVariable(struct Variable* var){
	
	freeSimpleVar(var->simpleVar);
	for(int i=0;i < var->count_memberAccessList; i++){
		freeVariable(var->memberAccessList[i]);
	}
	free(var->memberAccessList);
	free(var);
}
