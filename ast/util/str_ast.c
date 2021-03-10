#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "str_ast.h"

char* strBoolConst(struct BoolConst* bc){
	char* res =  malloc(sizeof(char)*10);
	strcpy(res, bc->value ? "true": "false");
	return res;
}	

char* strCharConst(struct CharConst* cc){

	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "'%c'", cc->value);
	return res;
}

char* strFloatConst(struct FloatConst* fc){

	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "%f", fc->value);
	return res;
}

char* strIdentifier(struct Identifier* id){

	char* res =  malloc(sizeof(char)*(strlen(id->identifier)+1));
	sprintf(res, "%s", id->identifier);
	return res;
}

char* strIntConst(struct IntConst* ic){

	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "%d", ic->value);
	return res;
}

char* strHexConst(struct HexConst* hc){

	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "0x%x", hc->value);
	return res;
}

char* strArrayType(struct ArrayType* at){

	char* inner = strType(at->element_type);
	
	char* res = malloc(sizeof(char)*(strlen(inner)+2+1));
	
	sprintf(res, "[%s]", inner);
	
	free(inner);
	
	return res;
}

char* strBasicTypeWrapped(struct BasicTypeWrapped* btw){
	
	if(btw->simpleType != NULL){
		return strSimpleType(btw->simpleType);
	}
	
	if(btw->subrType != NULL){
		return strSubrType(btw->subrType);
	}
	
	printf("(45)fatal Error (in str_ast.c)\n");
	exit(1);
	return NULL;
}

char* strSimpleType(struct SimpleType* st){
	
	if(st->primitiveType != NULL)
		{ return strPrimitiveType(st->primitiveType); }
		
	return strStructType(st->structType);
}

char* strSubrType(struct SubrType* st){

	//TODO: get rid of the magic number '1000'
	//and calculate how long exactly
	char* res = malloc(sizeof(char)*1000);
	strcpy(res, "");
	
	strcat(res, "(");
	
	for(int i=0;i < st->count_argTypes; i++){
	
		char* argType = strType(st->argTypes[i]);
		strcat(res, argType);
		free(argType);
		
		if(i < (st->count_argTypes - 1)){
			
			strcat(res, ", ");
		}
	}
	
	strcat(res, ")");
	
	strcat(res, (st->hasSideEffects)?"~>":"->");
	
	char* returntype = strType(st->returnType);
	strcat(res, returntype);
	free(returntype);
	
	return res;
}

char* strType(struct Type* t){
	
	if(t->m1 != NULL){ return strBasicTypeWrapped(t->m1); }
	
	if(t->m2 != NULL){ return strTypeParam(t->m2); }
	
	if(t->m3 != NULL){ return strArrayType(t->m3); }
	
	printf("fatal Error (in str_ast.c)\n");
	exit(1);
	return NULL;
}

char* strTypeParam(struct TypeParam* t){
	
	char* res =  malloc(sizeof(char)*10);
	sprintf(res, "?T%d", t->index);
	return res;
}


char* strPrimitiveType(struct PrimitiveType* p){
	
	char* res = malloc(DEFAULT_STR_SIZE);
	
	if(p->isIntType){
		//TODO: convert the enum to string,
		//for now just return 'int'
		sprintf(res, "int");
		exit(1);
	}
	
	if(p->isFloatType){ sprintf(res, "float"); }
	if(p->isCharType){  sprintf(res, "char"); }
	if(p->isBoolType){ sprintf(res, "bool"); }
	
	return res;
}

char* strStructType(struct StructType* s){
	
	//TODO: add the generic part
	if(s->typeParamCount != 0){
	
		printf("[AST][Error] strStructType\n");
		exit(1);
	}
	
	char* res = malloc(DEFAULT_STR_SIZE);
	
	sprintf(res, "%s", s->typeName);
	
	return res;
}
