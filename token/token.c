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
	res->value_ptr = 
		smalloc(sizeof(char)*(strlen(value)+1));
		
	strcpy(res->value_ptr, value);
	res->statically_allocated = false;
	
	res->lineNumber = -1;

	return res;
}
	
bool tokenEquals(struct Token* a, struct Token* b){
	const bool kindEq = a->kind == b->kind;
	if(kindEq){

		if(a->value_ptr != NULL && b->value_ptr != NULL){
			if(strlen(a->value_ptr)==0 || strlen(b->value_ptr)==0){
				return true;
			}else{
				return strcmp(a->value_ptr,b->value_ptr) == 0;
			}
		}
		return true;
	}
	return false;
}

void freeToken(struct Token* token){
	
	if(!token->statically_allocated){
		free(token->value_ptr);
	}
	free(token);
}
