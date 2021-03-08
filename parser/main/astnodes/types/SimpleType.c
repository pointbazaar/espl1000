#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SimpleType.h"

#include "ast/ast.h"

#include "token/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token.h"

void parse_type_params_rest(struct SimpleType* res, struct TokenList* tokens);

struct SimpleType* makeSimpleType2(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("SimpleType(...) from: ");
		list_print(tokens);
	}
	
	if(list_size(tokens) == 0){ return NULL; }

	struct SimpleType* res = make(SimpleType);
	
	res->typeParamCount = 0;
	res->isPrimitive    = false;
	
	
	
	strcpy(res->typeName, "");

	struct Token* next = list_head(tokens);
	
	res->isIntType   = next->kind == TYPEID_PRIMITIVE_INT;
	res->isFloatType = next->kind == TYPEID_PRIMITIVE_FLOAT;
	res->isCharType  = next->kind == TYPEID_PRIMITIVE_CHAR;
	
	switch(next->kind){
		
		case TYPEID_PRIMITIVE_INT:
		case TYPEID_PRIMITIVE_BOOL:
		case TYPEID_PRIMITIVE_CHAR:
		case TYPEID_PRIMITIVE_FLOAT:
		
			res->isPrimitive = true;
			
			strcpy(res->typeName, next->value_ptr);		
			list_consume(tokens, 1);
			return res;
			
		case TYPEID:
			res->isPrimitive = false;
			
			strcpy(res->typeName, next->value_ptr);
			list_consume(tokens, 1);
			
			if(list_size(tokens) < 3){ return res; }
			
			next = list_head(tokens);
			
			if(next->kind == OPKEY_RELATIONAL && strcmp(next->value_ptr, "<")==0 ){
				list_consume(tokens, 1);
				parse_type_params_rest(res, tokens);
			}	
			
			return res;	
			
		case ANYTYPE:
			
			//TODO: figure out what to do with 
			//res->isPrimitive
			//printf("dont know how to set res->isPrimitive\n");
			//exit(1);
			
			strcpy(res->typeName, "#");
			list_consume(tokens, 1);
			return res;
	}
	
	free(res);
	return NULL;	
}

struct SimpleType* makeSimpleType(char* typeName) {
	
	struct SimpleType* res = make(SimpleType);
	res->typeParamCount = 0;
	strcpy(res->typeName, typeName);

	return res;
}

void parse_type_params_rest(struct SimpleType* res, struct TokenList* tokens){
	
	uint32_t cap = 10; //capacity
	
	res->typeParams = malloc(sizeof(uint8_t) * cap);
	
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
		
		if(res->typeParamCount < cap){
			
			next = list_head(tokens);
			continue;
		}
			
		cap *= 2;
		
		const size_t newsize = sizeof(uint8_t) * cap;
		res->typeParams = realloc(res->typeParams, newsize);

		next = list_head(tokens);
	}
	
	next = list_head(tokens);
	
	//expect '>'
	const bool is_opkey = next->kind == OPKEY_RELATIONAL;
	const bool next_gt = strcmp(next->value_ptr, ">") == 0;
		
	if(is_opkey && next_gt){
		
		list_consume(tokens, 1);
		return;
	}
		
	printf("Syntax Error: expected '>', but got:\n");
	list_print(tokens);
	
	free(res);
	exit(1);
	
	return;
}
