#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <assert.h>

#include "util/ctx.h"

#include "gen_c_types.h"

char* translateIntType(enum INTTYPE type);
//----------------------------------------------------

char* simpleType2CType(struct SimpleType* s){
	
	assert(s != NULL);
	
	if(s->primitiveType != NULL){
		
		return primitiveType2CType(s->primitiveType);
	}
	
	if(s->structType != NULL){
		
		return structType2CType(s->structType);
	}

	printf("[Transpiler][Error]\n");
	exit(1);
}

char* structType2CType(struct StructType* s){
	
	assert(s != NULL);
	
	char* res = malloc(DEFAULT_STR_SIZE);
	
	if( strcmp(s->typeName, "String") == 0){
		strcpy(res, "char*");
		return res;
	}
	
	sprintf(res, "struct %s*", s->typeName);
	return res;
}

char* primitiveType2CType(struct PrimitiveType* p){

	assert(p != NULL);

	char* res = malloc(DEFAULT_STR_SIZE);
		
	if(p->isIntType){
		strcpy(res, translateIntType(p->intType));
		return res;
	}
	
	if(p->isFloatType){
		strcpy(res, "float"); return res;
	}
	
	if(p->isBoolType){
		strcpy(res, "bool"); return res;
	}
	
	if(p->isCharType){
		strcpy(res, "char"); return res;	
	}
	
	printf("[Transpiler][Error] in primitiveType2CType\n");
	exit(1);
	return NULL;
}

char* type2CType(struct Type* t, struct Ctx* ctx){
	
	if(t->m1 != NULL){
		return basicTypeWrapped2CType(t->m1, ctx);
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
	
	if(typeParam == NULL){}
	if(ctx       == NULL){}
	//TODO
	printf("[Transpiler][Error] transpileTypeParam not yet implemented!\n");
	exit(1);
	return NULL;
}

char* basicTypeWrapped2CType(struct BasicTypeWrapped* btw, struct Ctx* ctx){
	
	assert(btw != NULL);
	
	if(btw->simpleType != NULL){
		return simpleType2CType(btw->simpleType);
	}
	
	if(btw->subrType != NULL){
		return subrType2CType(btw->subrType, ctx);
	}
	
	printf("[Transpiler][Error]\n");
	exit(1);
	return NULL;	
}
//-------------------------------
bool isIntType(struct Type* t){
	
	if(t->m1 == NULL){ return false; }
	
	struct BasicTypeWrapped* m1 = t->m1;
	
	if(m1->simpleType == NULL){ return false; }
	
	struct SimpleType* s = m1->simpleType;
	
	if(s->primitiveType == NULL){ return false; }
	
	return s->primitiveType->isIntType;
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
