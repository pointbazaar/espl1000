#ifndef AST_TYPES_H
#define AST_TYPES_H

#include "../ast_declare.h"

#include "../sd_inttype.h"

struct ArrayType { 
	struct ASTNode super; 
	
	struct Type* element_type; 
};

struct BasicType {
	struct ASTNode super; 
	//these are alternatives,
	//only one of these is != NULL
	struct SimpleType* simpleType; //may be NULL
	struct SubrType* subrType; //may be NULL
};

struct PrimitiveType {
	struct ASTNode super; 

	//PType means 'Primitive Type'
	bool isIntType;
	bool isFloatType;
	bool isCharType;
	bool isBoolType;

	enum INTTYPE intType;
};

struct StructType {
	struct ASTNode super; 
	
	char typeName[DEFAULT_STR_SIZE];
	
	//the number of the type parameters
	uint8_t typeParamCount;
	uint8_t* typeParams;
};

struct SimpleType {
	struct ASTNode super; 

	struct PrimitiveType* primitiveType; //may be NULL

	struct StructType* structType; //may be NULL
};

struct SubrType {
	struct ASTNode super; 
	
	struct Type* returnType;
	
	struct Type** argTypes;
	uint8_t count_argTypes;
	
	bool hasSideEffects;
};

struct Type {
	struct ASTNode super; 
	
	//only one of these is != NULL
	struct BasicType* m1;
	struct TypeParam* m2;
	struct ArrayType* m3;
};

struct TypeParam {
	struct ASTNode super;  
	
	uint8_t index; /*type parameter index */ 
};

#endif
