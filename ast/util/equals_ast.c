#include <stdbool.h>
#include <string.h>

#include "../ast.h"

#include "equals_ast.h"

bool eq_type(struct Type* a, struct Type* b){
	
	if(a == NULL || b == NULL){ return false; }

	if(a->m1 != NULL)
		{ return eq_basictype(a->m1, b->m1); }
		
	if(a->m2 != NULL)
		{ return eq_typeparam(a->m2, b->m2); }
		
	if(a->m3 != NULL)
		{ return eq_arraytype(a->m3, b->m3); }
		
	return false;
}

bool eq_subrtype(struct SubrType* a, struct SubrType* b){
	
	if(a == NULL || b == NULL){ return false; }

	if(!eq_type(a->return_type, b->return_type))
		{ return false; }
		
	if(a->has_side_effects != b->has_side_effects)
		{ return false; }
		
	if(a->throws != b->throws)
		{ return false; }
		
	if(a->count_arg_types != b->count_arg_types)
		{ return false; }
		
	for(uint16_t i = 0; i < a->count_arg_types; i++){
		
		if(!eq_type(a->arg_types[i], b->arg_types[i]))
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

	if(a->primitive_type != NULL)
		{ return eq_primitivetype(a->primitive_type, b->primitive_type); }
		
	if(a->struct_type != NULL)
		{ return eq_structtype(a->struct_type, b->struct_type); }

	return false;
}

bool eq_structtype(struct StructType* a, struct StructType* b){

	if(a == NULL || b == NULL){ return false; }

	if(strcmp(a->type_name, b->type_name) != 0)
		{ return false; }
		
	if(a->count_type_params != b->count_type_params)
		{ return false; }
		
	for(uint16_t i = 0; i < a->count_type_params; i++){

	    if(!eq_type(a->type_params[i], b->type_params[i])){
	        return false;
	    }
	}
	
	return true;
}

bool eq_primitivetype(struct PrimitiveType* a, struct PrimitiveType* b){
	
	if(a == NULL || b == NULL){ return false; }

	if(a->is_char_type && b->is_char_type){ return true; }
	if(a->is_float_type && b->is_float_type){ return true; }
	if(a->is_bool_type && b->is_bool_type){ return true; }
	
	if(a->is_int_type && b->is_int_type){
		if(a->int_type == b->int_type)
			{ return true; }
	}

	return false;
}

bool eq_basictype(struct BasicType* a, struct BasicType* b){
	
	if(a == NULL || b == NULL){ return false; }

	if(a->simple_type != NULL)
		{ return eq_simpletype(a->simple_type, b->simple_type); }
		
	if(a->subr_type != NULL)
		{ return eq_subrtype(a->subr_type, b->subr_type); }

	return false;
}

bool eq_arraytype(struct ArrayType* a, struct ArrayType* b){
	
	if(a == NULL || b == NULL){ return false; }
	
	return eq_type(a->element_type, b->element_type);
}
