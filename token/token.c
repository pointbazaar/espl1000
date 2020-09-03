#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "token.h"
#include "../util/util.h"

struct Token* makeToken(int kind){
	return makeToken2(kind,"");
}

struct Token* makeToken2(int kind, char* value){
	
	struct Token* res = smalloc(sizeof(struct Token));

	res->kind = kind;
	strncpy(res->value, value, 20);
	res->lineNumber = -1;

	return res;
}
	
bool tokenEquals(struct Token* a, struct Token* b){
	const bool kindEq = a->kind == b->kind;
	if(kindEq){

		if(a->value != NULL && b->value != NULL){
			if(strlen(a->value)==0 || strlen(b->value)==0){
				return true;
			}else{
				return strcmp(a->value,b->value) == 0;
			}
		}
		return true;
	}
	return false;
}

void freeToken(struct Token* token){
	
	free(token);
}
