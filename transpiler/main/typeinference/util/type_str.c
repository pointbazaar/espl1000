#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables/symtable/symtable.h"
#include "tables/lvst/lvst.h"
#include "tables/sst/sst.h"
#include "tables/stst/stst.h"

#include "ast/ast.h"
#include "ast/util/str_ast.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"


struct Type* typeFromStrPrimitive(struct ST* st, char* typeName){
	
	struct Type* res             = make(Type);
	struct BasicTypeWrapped* btw = make(BasicTypeWrapped);
	struct SimpleType* s         = make(SimpleType);
	struct PrimitiveType* p      = make(PrimitiveType);
	
	res->m1 = btw;
	res->m2 = NULL;
	res->m3 = NULL;
	
	p->isIntType   = strcmp(typeName, "int")   == 0;
	p->isFloatType = strcmp(typeName, "float") == 0;
	p->isCharType  = strcmp(typeName, "char")  == 0;
	p->isBoolType  = strcmp(typeName, "bool")  == 0;
	
	p->intType = INT;

	s->primitiveType = p;
	s->structType    = NULL;

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
	res->m3 = NULL;
	
	struct SimpleType* simpleType = make(SimpleType);
	struct StructType* structType = make(StructType);
	
	simpleType->primitiveType = NULL;
	simpleType->structType    = structType;

	structType->typeParamCount = 0;
	strncpy(structType->typeName, typeName, DEFAULT_STR_SIZE);
	
	btw->subrType = NULL;
	btw->simpleType = simpleType;
	
	registerInferredType(st, res);
	
	return res;
}

void registerInferredType(struct ST* st, struct Type* t){
	
	//[1]
	if(st->inferredTypesCount >= st->inferredTypesCapacity){
		print_exit("NO FILENAME", &(t->super), "Fatal Error (in type_str.c). too many inferred types\n");
	}else{
		st->inferredTypes[st->inferredTypesCount] = t;
		st->inferredTypesCount += 1;
	}
}
