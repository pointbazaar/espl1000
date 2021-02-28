#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables/symtable.h"
#include "tables/localvar_symtable.h"
#include "tables/subr_symtable.h"
#include "tables/struct_symtable.h"

#include "ast/ast.h"

#include "typeinference/type_str.h"


char* typeToStr(struct Type* t){
	
	if(t->m1 != NULL){
		return typeToStrBasicTypeWrapped(t->m1);
	}
	
	printf("(1)currently not implemented (in typeinference.c)\n");
	exit(1);
	return NULL;
}

char* typeToStrBasicTypeWrapped(struct BasicTypeWrapped* b){
	
	if(b->simpleType != NULL){
		
		return b->simpleType->typeName;
	}
	
	printf("(2)currently not implemented (in typeinference.c)\n");
	exit(1);
	return NULL;	
}


struct Type* typeFromStr(struct ST* st, char* typeName, bool isPrimitive, bool isIntType){
	
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
		printf("Fatal Error (in typeinference.c)\n");
		exit(1);
	}else{
		st->inferredTypes[st->inferredTypesCount] = res;
		st->inferredTypesCount += 1;
	}
	
	return res;
}
