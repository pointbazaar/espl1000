#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables/symtable.h"
#include "tables/lvst.h"
#include "tables/sst.h"
#include "tables/stst.h"

#include "ast/ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/type_str.h"

struct Type* typeFromStr(struct ST* st, char* typeName, bool isPrimitive, bool isIntType){
	
	//TODO: we should really be using
	//the lexer and parser here
	
	//this method will only work for simple types
	struct Type* res = malloc(sizeof(struct Type));
	
	struct BasicTypeWrapped* btw = malloc(sizeof(struct BasicTypeWrapped));
	
	res->m1 = btw;
	res->m2 = NULL;
	res->m2 = NULL;
	
	struct SimpleType* simpleType = malloc(sizeof(struct SimpleType));
	
	simpleType->isPrimitive = isPrimitive;
	simpleType->isIntType = isIntType;

	simpleType->typeParamCount = 0;
	strncpy(simpleType->typeName, typeName, DEFAULT_STR_SIZE);
	
	btw->subrType = NULL;
	btw->simpleType = simpleType;
	
	//register the pointer in SymbolTable 'st'
	//so it can manage that memory,
	//as this pointer is not part of the AST Tree
	
	if(st->inferredTypesCount >= st->inferredTypesCapacity){
		printf("Fatal Error (in type_str.c)\n");
		exit(1);
	}else{
		st->inferredTypes[st->inferredTypesCount] = res;
		st->inferredTypesCount += 1;
	}
	
	return res;
}
