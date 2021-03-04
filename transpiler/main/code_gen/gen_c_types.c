#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>

#include "util/ctx.h"

#include "gen_c_types.h"

char* translateIntType(char* type);
//----------------------------------------------------

char* simpleType2CType(struct SimpleType* simpleType){
	
	char* tname = simpleType->typeName;
	
	char* res = malloc(sizeof(char)*DEFAULT_STR_SIZE*3);
	strcpy(res, "");
	
	if(simpleType->isPrimitive){
		if(simpleType->isIntType){
				
			strcpy(res, translateIntType(tname));
			
		}else if(strcmp(tname, "float") == 0){
			strcpy(res, "float");
			
		}else if( strcmp(tname, "bool") == 0 ){
			strcpy(res, "bool");
			
		}else if( strcmp(tname, "char") == 0){
			strcpy(res, "char");	
		}else{
			
			printf("Error in gen_c_types.c (simpleType2CType):\n");
			printf("primitive type %s could not be transpiled\n", tname);
			exit(1);
		}
		
	}else{
		
		if( strcmp(tname, "String") == 0){
			strcpy(res, "char*");
			
		}else{
		
			sprintf(res, "struct %s*", tname);
		}
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
	}
	
	if(btw->subrType != NULL){
		return subrType2CType(btw->subrType, ctx);
	}
	
	printf("FATAL ERROR\n");
	exit(1);
	return NULL;	
}
//-------------------------------
bool isIntType(struct Type* t){
	
	if(t->m1 == NULL){ return false; }
	
	struct BasicTypeWrapped* m1 = t->m1;
	if(m1->simpleType == NULL){ return false; }
	
	struct SimpleType* s = m1->simpleType;
	
	return s->isIntType;
}
char* translateIntType(char* type){
	char* types_def_width[] = 
	{"int8","int16","int32","int64",
	"uint8","uint16","uint32","uint64"};
	
	char* map[] = 
	{"int8_t","int16_t","int32_t","int64_t",
	"uint8_t","uint16_t","uint32_t","uint64_t"};
	
	for(int i=0;i < 8; i++){
		if(strcmp(type, types_def_width[i]) == 0){ 
			return map[i]; 
		}
	}
	return "int";
}
//-----------------------------------
char* typeNameToCFormatStr(char* typeName){
	
	//int l = strlen(typeName);
	
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
