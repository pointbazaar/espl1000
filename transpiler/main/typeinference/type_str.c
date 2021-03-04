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
	
	if(t->m1 != NULL){ return typeToStrBasicTypeWrapped(t->m1); }
	
	if(t->m2 != NULL){
		printf("(1)currently not implemented (in typeinference.c)\n");
		exit(1);
	}
	
	if(t->m3 != NULL){ return typeToStrArrayType(t->m3); }
	
	printf("fatal Error (in typeinference.c)\n");
	exit(1);
	return NULL;
}

char* typeToStrBasicTypeWrapped(struct BasicTypeWrapped* b){
	
	if(b->simpleType != NULL){
		
		return b->simpleType->typeName;
	}
	
	if(b->subrType != NULL){
		
		return typeToStrSubrType(b->subrType);
	}
	
	printf("(45)fatal Error (in typeinference.c)\n");
	exit(1);
	return NULL;	
}

char* typeToStrArrayType(struct ArrayType* t){
	
	char* inner = typeToStr(t->element_type);
	
	char* res = malloc(sizeof(char)*(strlen(inner)+2+1));
	
	sprintf(res, "[%s]", inner);
	
	free(inner);
	
	return res;
}

char* typeToStrSubrType(struct SubrType* t){
	
	//TODO: get rid of the magic number '1000'
	//and calculate how long exactly
	char* res = malloc(sizeof(char)*1000);
	strcpy(res, "");
	
	strcat(res, "(");
	
	for(int i=0;i < t->count_argTypes; i++){
	
		char* argType = typeToStr(t->argTypes[i]);
		strcat(res, argType);
		free(argType);
		
		if(i < (t->count_argTypes - 1)){
			
			strcat(res, ", ");
		}
	}
	
	strcat(res, ")");
	
	strcat(res, (t->hasSideEffects)?"~>":"->");
	
	char* returntype = typeToStr(t->returnType);
	strcat(res, returntype);
	free(returntype);
	
	return res;
}

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
		printf("Fatal Error (in typeinference.c)\n");
		exit(1);
	}else{
		st->inferredTypes[st->inferredTypesCount] = res;
		st->inferredTypesCount += 1;
	}
	
	return res;
}
