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
	struct SimpleType* simple_type; //may be NULL
	struct SubrType* subr_type; //may be NULL
};

struct PrimitiveType {
	struct ASTNode super; 

	//PType means 'Primitive Type'
	bool is_int_type;
	bool is_char_type;
	bool is_bool_type;

	enum INTTYPE int_type;
};

struct StructType {

	struct ASTNode super;
	char type_name[DEFAULT_STR_SIZE];
};

struct SimpleType {
	struct ASTNode super; 

	struct PrimitiveType* 	primitive_type; //may be NULL
	struct StructType* 		struct_type; 	//may be NULL
};

struct SubrType {
	struct ASTNode super; 
	
	struct Type* return_type;
	
	struct Type** arg_types;
	uint8_t count_arg_types;
	
	bool has_side_effects;
};

struct Type {
	struct ASTNode super; 
	
	//only one of these is != NULL
	struct BasicType* basic_type;
	struct TypeParam* type_param;
	struct ArrayType* array_type;
};

struct TypeParam {
	struct ASTNode super;  
	
	uint8_t index; /*type parameter index */ 
};

#endif
