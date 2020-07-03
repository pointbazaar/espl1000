#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "Token.h"

struct Token* makeToken(int kind){
	return makeToken2(kind,"");
}

struct Token* makeToken2(int kind, char* value){
	
	struct Token* res = malloc(sizeof(struct Token));
	if(res == NULL){ exit(1); }

	res->kind = kind;
	strncpy(res->value, value, 20);
	res->lineNumber = -1;

	return res;
}
	
bool tokenEquals(struct Token* a, struct Token* b){
	if( strlen(a->value)==0 
		|| strlen(b->value)==0
	){
		return a->kind == b->kind;
	}

	return a->kind == b->kind 
		&& strcmp(a->value,b->value) == 0;
}
