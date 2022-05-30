#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "util/ctx.h"

#include "tables/stst/stst.h"
#include "tables/symtable/symtable.h"

#include "gen_c_types.h"

char* translateIntType(enum INTTYPE type);
//----------------------------------------------------

char *simpleType2CType(struct SimpleType *s, struct STST *stst) {
	
	if(s->primitive_type != NULL){
		
		return primitiveType2CType(s->primitive_type);
	}
	
	if(s->struct_type != NULL){
		
		return structType2CType(s->struct_type, stst);
	}

	printf("[Transpiler][Error]\n");
	exit(1);
}

char* structType2CType(struct StructType* s, struct STST* stst){

	char* res = malloc(DEFAULT_STR_SIZE);
    strcpy(res, "");

    struct STSTLine* line = stst_get(stst, s->type_name);

    if(line->type_name_in_c != NULL){
        strcat(res, line->type_name_in_c);
    }else{
        strcat(res, "struct ");
        strcat(res, s->type_name);
    }

    strcat(res, "*");
	return res;
}

char* primitiveType2CType(struct PrimitiveType* p){

	char* res = malloc(DEFAULT_STR_SIZE);
		
	if(p->is_int_type){
		strcpy(res, translateIntType(p->int_type));
		return res;
	}
	
	if(p->is_float_type){
		strcpy(res, "float"); return res;
	}
	
	if(p->is_bool_type){
		strcpy(res, "bool"); return res;
	}
	
	if(p->is_char_type){
		strcpy(res, "char"); return res;	
	}
	
	printf("[Transpiler][Error] in primitiveType2CType\n");
	exit(1);
	return NULL;
}

char* type2CType(struct Type* t, struct Ctx* ctx){
	
	if(t->basic_type != NULL){
		return basicType2CType(t->basic_type, ctx);
	}
	
	if(t->type_param != NULL){
		return typeParam2CType(t->type_param, ctx);
	}
	
	if(t->array_type != NULL){
		return arrayType2CType(t->array_type, ctx);
	}

	printf("[Transpiler][Error] in type2CType\n");
	exit(1);
	return NULL;
}

char* arrayType2CType(struct ArrayType* atype, struct Ctx* ctx){
	
	char* res = malloc(sizeof(char)*DEFAULT_STR_SIZE*3);
	
	char* res1 = type2CType(atype->element_type, ctx);
	strcpy(res, res1);
	free(res1);
	
	strcat(res, "*");
	return res;
}

char* subrType2CType(struct SubrType* subrType, struct Ctx* ctx){
	
	char* res = malloc(sizeof(char)*DEFAULT_STR_SIZE*3);
	
	//https://www.zentut.com/c-tutorial/c-function-pointer/
	//return type
	char* res1 = type2CType(subrType->return_type, ctx);
	strcpy(res, res1);
	free(res1);
	
	//function_ptr should be the name of the
	//variable/argument that holds the function pointer
	char temp[DEFAULT_STR_SIZE*3];
	sprintf(temp, "(*%s) (", ctx->current_function_pointer_var_or_arg_name);
	strcat(res, temp);

	//arguments
	for(int i=0; i < subrType->count_arg_types; i++){
		char* res2 = type2CType(subrType->arg_types[i], ctx);
		strcat(res, res2);
		free(res2);
		
		if(i < (subrType->count_arg_types) - 1){
			strcat(res, ", ");
		}
	}
	strcat(res, ")");
	
	return res;
}

char* typeParam2CType(){

    char* res = malloc(DEFAULT_STR_SIZE);
    strcpy(res, "void*");
    return res;
}

char* basicType2CType(struct BasicType* btw, struct Ctx* ctx){
	
	if(btw->simple_type != NULL){
		return simpleType2CType(btw->simple_type, ctx->tables->stst);
	}
	
	if(btw->subr_type != NULL){
		return subrType2CType(btw->subr_type, ctx);
	}
	
	printf("[Transpiler][Error]\n");
	exit(1);
	return NULL;	
}
//-------------------------------
char* translateIntType(enum INTTYPE type){
	
	char* map[] = 
	{"int8_t","int16_t","int32_t","int64_t",
	"uint8_t","uint16_t","uint32_t","uint64_t",
	"int", "uint32_t"
	};
	
	return map[type];
}
