#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SimpleType.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../ast/free_ast.h"
#include "../../../../token/token.h"

void parse_type_params_rest(struct SimpleType* res, struct TokenList* tokens);

struct SimpleType* makeSimpleType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleType(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){ return NULL; }

	struct SimpleType* res = malloc(sizeof(struct SimpleType));
	res->typeParamCount = 0;
	strcpy(res->typeName, "");

	struct Token* next = list_head(tokens);
	
	if (next->kind == TYPEIDENTIFIER) {
		
		strcpy(res->typeName, next->value_ptr);
		list_consume(tokens, 1);
		
		if(list_size(tokens) < 3){ return res; }
		
		next = list_head(tokens);
		
		if(next->kind == OPKEY && strcmp(next->value_ptr, "<")==0 ){
			list_consume(tokens, 1);
			parse_type_params_rest(res, tokens);
		}	
		
		return res;	
	}
	
	if (next->kind == ANYTYPE) {
		
		strcpy(res->typeName, "#");
		list_consume(tokens, 1);
		return res;
	}
	
	free(res);
	return NULL;	
}

struct SimpleType* makeSimpleType(char* typeName) {
	struct SimpleType* res = malloc(sizeof(struct SimpleType));
	res->typeParamCount = 0;
	strcpy(res->typeName, typeName);

	return res;
}

void parse_type_params_rest(struct SimpleType* res, struct TokenList* tokens){
	
	//we must allocate space for those type parameters
	int capacity = 10;
	res->typeParams = malloc(sizeof(uint8_t)*capacity);
	
	//expect ?TX
	struct Token* next = list_head(tokens);
	if(next->kind != TPARAM){
		printf("Expected Type Parameter, got:\n");
		list_print(tokens);
		exit(1);
	}
	res->typeParams[res->typeParamCount] = atoi(next->value_ptr);
	res->typeParamCount += 1;
	list_consume(tokens, 1);
	
	next = list_head(tokens);
	while(next->kind == COMMA){
		list_consume(tokens, 1);
		
		if(next->kind != TPARAM){
			printf("Expected Type Parameter, got:\n");
			list_print(tokens);
			exit(1);
		}
		res->typeParams[res->typeParamCount] = atoi(next->value_ptr);
		res->typeParamCount += 1;
		list_consume(tokens, 1);
		
		//realloc if necessary
		if(res->typeParamCount >= capacity){
			capacity *= 2;
			res->typeParams = 
				realloc(
					res->typeParams, 
					sizeof(uint8_t)*capacity
				);
		}
		
		next = list_head(tokens);
	}
	
	next = list_head(tokens);
	//expect '>'
	const bool condition = 
		next->kind == OPKEY 
		&& strcmp(next->value_ptr, ">")==0;
		
	if(!condition){
		printf("Syntax Error: expected '>', but got:\n");
		list_print(tokens);
		free(res);
		exit(1);
	}
	
	list_consume(tokens, 1);
}
