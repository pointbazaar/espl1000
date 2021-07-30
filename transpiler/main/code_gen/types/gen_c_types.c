#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "util/ctx.h"

#include "../../tables/stst/stst.h"
#include "../../tables/symtable/symtable.h"

#include "gen_c_types.h"

char* translateIntType(enum INTTYPE type);
//----------------------------------------------------

char *simpleType2CType(struct SimpleType *s, struct STST *stst) {
	
	assert(s != NULL);
	
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

	assert(p != NULL);

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
	
	if(t->m1 != NULL){
		return basicType2CType(t->m1, ctx);
	}
	
	if(t->m2 != NULL){
		return typeParam2CType(t->m2, ctx);
	}
	
	if(t->m3 != NULL){
		return arrayType2CType(t->m3, ctx);
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

char* typeParam2CType(struct TypeParam* typeParam, struct Ctx* ctx){
	
	if(typeParam == NULL){}
	if(ctx       == NULL){}
	//TODO
	printf("[Transpiler][Error] transpileTypeParam not yet implemented!\n");
	exit(1);
	return NULL;
}

char* basicType2CType(struct BasicType* btw, struct Ctx* ctx){
	
	assert(btw != NULL);
	
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
bool isIntType(struct Type* t){
	
	if(t->m1 == NULL){ return false; }
	
	struct BasicType* m1 = t->m1;
	
	if(m1->simple_type == NULL){ return false; }
	
	struct SimpleType* s = m1->simple_type;
	
	if(s->primitive_type == NULL){ return false; }
	
	return s->primitive_type->is_int_type;
}
char* translateIntType(enum INTTYPE type){
	
	assert(type > NONE);
	assert(type < INTTYPE_END);
	
	char* map[] = 
	{"int8_t","int16_t","int32_t","int64_t",
	"uint8_t","uint16_t","uint32_t","uint64_t",
	"int", "uint32_t"
	};
	
	return map[type];
}
//-----------------------------------
char* typeNameToCFormatStr(char* typeName){
	
	if(strcmp(typeName, "int") == 0
	|| strcmp(typeName, "uint") == 0
	|| strcmp(typeName, "int8") == 0
	|| strcmp(typeName, "uint8") == 0
	|| strcmp(typeName, "int16") == 0
	|| strcmp(typeName, "uint16") == 0
	|| strcmp(typeName, "int32") == 0
	|| strcmp(typeName, "uint32") == 0
	|| strcmp(typeName, "int64") == 0
	|| strcmp(typeName, "uint64") == 0
	|| strcmp(typeName, "bool") == 0
	){
		return "%d";
	}else if(strcmp(typeName, "char") == 0){
		return "%c";
	}else if(strcmp(typeName, "String") == 0){
		return "%s";
	}else if(strcmp(typeName, "float") == 0){
		return "%f";
	}
	
	return "%p";
}
