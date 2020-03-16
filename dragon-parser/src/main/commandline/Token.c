#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Token.h"

struct Token* makeToken(int kind){
	return makeToken(kind,"");
}

struct Token* makeToken2(int kind, char* value){
	struct Token* res = smalloc(sizeof(struct Token));
	res->kind = kind;
	res->value = value;
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
		&& strcmp(a->value,other->value) == 0;
}