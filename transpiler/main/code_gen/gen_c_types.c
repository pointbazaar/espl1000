#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>

#include "../ctx.h"
#include "gen_c_types.h"

char* translateIntType(char* type);
//----------------------------------------------------

char* simpleType2CType(struct SimpleType* simpleType){
	
	//type name
	char* t = simpleType->typeName;
	char* res = malloc(sizeof(char)*DEFAULT_STR_SIZE*3);
	strcpy(res, "");

	if(isIntType(t)){
		
		strcpy(res, translateIntType(t));
		
	}else if(
		   strcmp(t, "PFloat") == 0
		|| strcmp(t, "NFloat") == 0
		|| strcmp(t, "Float") == 0
	){
		strcpy(res, "float");
		
	}else if( strcmp(t, "Bool") == 0 ){
		strcpy(res, "bool");
		
	}else if( strcmp(t, "String") == 0){
		strcpy(res, "char*");
		
	}else if( strcmp(t, "Char") == 0){
		strcpy(res, "char");
		
	}else{
		//if we do not recognize it, treat it as struct pointer
		sprintf(res, "struct %s*", t);
	}
	
	return res;
}

char* type2CType(struct Type* t, struct Ctx* ctx){
	
	if(t->m1 != NULL){
		return basicTypeWrapped2CType(t->m1, ctx);
	}else if(t->m2 != NULL){
		return typeParam2CType(t->m2, ctx);
	}else if(t->m3 != NULL){
		return arrayType2CType(t->m3, ctx);
	}else{
		printf("FATAL ERROR\n");
		exit(1);
	}
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
	char* res1 = type2CType(subrType->returnType, ctx);
	strcpy(res, res1);
	free(res1);
	
	//function_ptr should be the name of the
	//variable/argument that holds the function pointer
	char temp[DEFAULT_STR_SIZE*3];
	sprintf(temp, "(*%s) (", ctx->currentFunctionPointerVarOrArgName);
	strcat(res, temp);

	//arguments
	for(int i=0; i < subrType->count_argTypes; i++){
		char* res2 = type2CType(subrType->argTypes[i], ctx);
		strcat(res, res2);
		free(res2);
		
		if(i < (subrType->count_argTypes)-1){
			strcat(res, ", ");
		}
	}
	strcat(res, ")");
	
	return res;
}

char* typeParam2CType(struct TypeParam* typeParam, struct Ctx* ctx){
	
	//TODO
	printf("transpileTypeParam not yet implemented!\n");
	exit(1);
	return NULL;
}

char* basicTypeWrapped2CType(struct BasicTypeWrapped* btw, struct Ctx* ctx){
	
	if(btw->simpleType != NULL){
		return simpleType2CType(btw->simpleType);
	}else if(btw->subrType != NULL){
		return subrType2CType(btw->subrType, ctx);
	}else{
		printf("FATAL ERROR\n");
		exit(1);
		return NULL;
	}
}
//-------------------------------
bool isIntType(char* t){
	char* types[] = 
	{
		"PInt","NInt","Int","NZInt",
		"Int8","Int16","Int32","Int64",
	};
	
	for(int i=0;i < 8; i++){
		if(strcmp(t, types[i]) == 0){ return true; }
	}
	return false;
}
char* translateIntType(char* type){
	char* types_def_width[] = 
	{"Int8","Int16","Int32","Int64"};
	
	char* map[] = 
	{"int8_t","int16_t","int32_t","int64_t"};
	
	for(int i=0;i < 4; i++){
		if(strcmp(type, types_def_width[i]) == 0){ 
			return map[i]; 
		}
	}
	return "int";
}
