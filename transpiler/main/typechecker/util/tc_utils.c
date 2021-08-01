#include <stdbool.h>
#include <string.h>

//AST Includes
#include "ast/ast.h"

//Typechecker Includes
#include "tc_utils.h"

bool is_primitive_type(struct Type* type){
	
	if(type->m1 == NULL){ return false; }
	if(type->m1->simple_type == NULL){ return false; }
	
	if(type->m1->simple_type->primitive_type == NULL)
		{ return false; }
		
	return true;
}

bool is_float_type(struct Type* type){

    if(!is_primitive_type(type)){ return false; }

    return type->m1->simple_type->primitive_type->is_float_type;
}

bool is_integer_type(struct Type* type){
	
	if(!is_primitive_type(type)){ return false; }
	
	return type->m1->simple_type->primitive_type->is_int_type;
}

bool is_bool_type(struct Type* type){
	
	if(!is_primitive_type(type)){ return false; }
	
	return type->m1->simple_type->primitive_type->is_bool_type;
}

bool is_char_type(struct Type* type){
	
	if(!is_primitive_type(type)){ return false; }
	
	return type->m1->simple_type->primitive_type->is_char_type;
}

bool is_malloc(struct Expr* expr){
	
	if(expr->term2 != NULL){ return false; }
	
	if(expr->term1->op != NULL){ return false; }
	
	//TODO: use enum or constant not hardcoded magic num
	if(expr->term1->term->kind != 4){ return false; }
	
	char* name = expr->term1->term->ptr.m4->name;
	
	return strcmp(name, "malloc") == 0;
}

uint32_t max_indices_allowed(struct Type* type){

    if(type->m3 != NULL){
        return 1 + max_indices_allowed(type->m3->element_type);
    }

    return 0;
}
