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

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

//  [1]
/*  register the pointer in SymbolTable 'st'
 *	so it can manage that memory,
 *	as this pointer is not part of the AST Tree
 */
 
static void registerInferredType(struct ST* st, struct Type* t);

struct Type* typeFromStrPrimitive(struct ST* st, char* typeName){
	
	struct Type* res             = make(Type);
	struct BasicTypeWrapped* btw = make(BasicTypeWrapped);
	struct SimpleType* s         = make(SimpleType);
	
	res->m1 = btw;
	res->m2 = NULL;
	res->m2 = NULL;
	
	s->isPrimitive = true;
	
	s->isIntType   = strcmp(typeName, "int")   == 0;
	s->isFloatType = strcmp(typeName, "float") == 0;
	s->isCharType  = strcmp(typeName, "char")  == 0;
	s->isBoolType  = strcmp(typeName, "bool")  == 0;

	s->typeParamCount = 0;
	strncpy(s->typeName, typeName, DEFAULT_STR_SIZE);
	
	btw->subrType = NULL;
	btw->simpleType = s;
	
	registerInferredType(st, res);
	
	return res;
}

struct Type* typeFromStr(struct ST* st, char* typeName){
	
	//this method will only work for simple types,
	//not subroutine types or array types.
	
	struct Type* res = make(Type);
	
	struct BasicTypeWrapped* btw = make(BasicTypeWrapped);
	
	res->m1 = btw;
	res->m2 = NULL;
	res->m2 = NULL;
	
	struct SimpleType* simpleType = make(SimpleType);
	
	simpleType->isPrimitive = false;
	
	simpleType->isIntType   = false;
	simpleType->isFloatType = false;
	simpleType->isCharType  = false;
	simpleType->isBoolType  = false;


	simpleType->typeParamCount = 0;
	strncpy(simpleType->typeName, typeName, DEFAULT_STR_SIZE);
	
	btw->subrType = NULL;
	btw->simpleType = simpleType;
	
	registerInferredType(st, res);
	
	return res;
}

static void registerInferredType(struct ST* st, struct Type* t){
	
	//[1]
	if(st->inferredTypesCount >= st->inferredTypesCapacity){
		print_exit("Fatal Error (in type_str.c)\n");
	}else{
		st->inferredTypes[st->inferredTypesCount] = t;
		st->inferredTypesCount += 1;
	}
}
