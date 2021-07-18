#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "token.h"
#include "../TokenKeys.h"

struct Token* makeToken(int kind){
	return makeToken2(kind,"");
}

struct Token* makeToken2(int kind, char* value){
	
	struct Token* res = malloc(sizeof(struct Token));

	res->kind = kind;
	
	res->value_ptr = malloc(sizeof(char)*(strlen(value)+1));
		
	strcpy(res->value_ptr, value);
	
	res->line_num = -1;

	return res;
}

struct Token* makeTokenStringConst(char* value){
	struct Token* res = malloc(sizeof(struct Token));

	res->kind = STRINGCONST;
	
	res->value_ptr = malloc(sizeof(char)*(strlen(value)+1));
		
	//extra '"' at start and end
	strcpy(res->value_ptr, value+1);
	res->value_ptr[strlen(res->value_ptr)-1] = '\0';
	
	res->line_num = -1;

	return res;
}
	
bool token_equals(struct Token* a, struct Token* b){
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
	
	free(token->value_ptr);
	free(token);
}
