#ifndef AST_TYPES_H
#define AST_TYPES_H

#include "../ast_declare.h"

struct ArrayType { 
	
	struct Type* element_type; 
};

struct BasicTypeWrapped {
	//these are alternatives,
	//only one of these is != NULL
	struct SimpleType* simpleType; //may be NULL
	struct SubrType* subrType; //may be NULL
};

struct SimpleType {
	
	char typeName[DEFAULT_STR_SIZE];
	
	bool isPrimitive; //is it a primitive data type?
	bool isIntType;
	
	//the number of the type parameters
	uint8_t typeParamCount;
	uint8_t* typeParams;
};

struct SubrType {
	struct Type* returnType;
	
	struct Type** argTypes;
	uint8_t count_argTypes;
	
	bool hasSideEffects;
};

struct Type {
	//only one of these is != NULL
	struct BasicTypeWrapped* m1;
	struct TypeParam* m2;
	struct ArrayType* m3;
};

struct TypeParam { 
	
	uint8_t index; /*type parameter index */ 
};

#endif
