#include <stdbool.h>
#include <string.h>

#include "../ast.h"

#include "equals_ast.h"

bool eq_type(struct Type* a, struct Type* b){
	
	if(a == NULL || b == NULL){ return false; }

	if(a->m1 != NULL)
		{ return eq_basictypewrapped(a->m1, b->m1); }
		
	if(a->m2 != NULL)
		{ return eq_typeparam(a->m2, b->m2); }
		
	if(a->m3 != NULL)
		{ return eq_arraytype(a->m3, b->m3); }
		
	return false;
}

bool eq_subrtype(struct SubrType* a, struct SubrType* b){
	
	if(a == NULL || b == NULL){ return false; }

	if(!eq_type(a->returnType, b->returnType))
		{ return false; }
		
	if(a->hasSideEffects != b->hasSideEffects)
		{ return false; }
		
	if(a->count_argTypes != b->count_argTypes)
		{ return false; }
		
	for(uint16_t i = 0; i < a->count_argTypes; i++){
		
		if(!eq_type(a->argTypes[i], b->argTypes[i]))
			{ return false; }
	}
	
	return true;
}

bool eq_typeparam(struct TypeParam* a, struct TypeParam* b){
	
	if(a == NULL || b == NULL){ return false; }
	
	return a->index == b->index;
}

bool eq_simpletype(struct SimpleType* a, struct SimpleType* b){

	if(a == NULL || b == NULL){ return false; }

	if(a->primitiveType != NULL)
		{ return eq_primitivetype(a->primitiveType, b->primitiveType); }
		
	if(a->structType != NULL)
		{ return eq_structtype(a->structType, b->structType); }

	return false;
}

bool eq_structtype(struct StructType* a, struct StructType* b){

	if(a == NULL || b == NULL){ return false; }

	if(strcmp(a->typeName, b->typeName) != 0)
		{ return false; }
		
	if(a->typeParamCount != b->typeParamCount)
		{ return false; }
		
	for(uint16_t i = 0; i < a->typeParamCount; i++){
	
		if(a->typeParams[i] != b->typeParams[i])
			{ return false; }
	}
	
	return true;
}

bool eq_primitivetype(struct PrimitiveType* a, struct PrimitiveType* b){
	
	if(a == NULL || b == NULL){ return false; }

	if(a->isCharType && b->isCharType){ return true; }
	if(a->isFloatType && b->isFloatType){ return true; }
	if(a->isBoolType && b->isBoolType){ return true; }
	
	if(a->isIntType && b->isIntType){ 
		if(a->intType == b->intType)
			{ return true; }
	}

	return false;
}

bool eq_basictypewrapped(struct BasicTypeWrapped* a, struct BasicTypeWrapped* b){
	
	if(a == NULL || b == NULL){ return false; }

	if(a->simpleType != NULL)
		{ return eq_simpletype(a->simpleType, b->simpleType); }
		
	if(a->subrType != NULL)
		{ return eq_subrtype(a->subrType, b->subrType); }

	return false;
}

bool eq_arraytype(struct ArrayType* a, struct ArrayType* b){
	
	if(a == NULL || b == NULL){ return false; }
	
	return eq_type(a->element_type, b->element_type);
}
