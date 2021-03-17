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

char* strStringConst(struct StringConst* s){
	
	char* res =  malloc(sizeof(char)*(3+strlen(s->value)));
	sprintf(res, "\"%s\"", s->value);
	return res;
}

char* strBinConst(struct BinConst* b){
	
	//TODO: implement
	char* res = malloc(sizeof(char)*10);
	strcpy(res, "TODO");
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
	
	printf("[AST][Error] (45)fatal (in str_ast.c)\n");
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
		
		char* types[] = 
		{"int8","int16","int32","int64",
		"uint8","uint16","uint32","uint64",
		"int","uint"
		};
		
		sprintf(res, "%s", types[p->intType]);
	}
	
	if(p->isFloatType) { sprintf(res, "float"); }
	if(p->isCharType)  { sprintf(res, "char"); }
	if(p->isBoolType)  { sprintf(res, "bool"); }
	
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

char* strVariable(struct Variable* v){
	
	char* s1 = strSimpleVar(v->simpleVar);
	
	char* s2 = (v->memberAccess != NULL)?strVariable(v->memberAccess):"";
	
	uint16_t l = strlen(s1) + strlen(s2);
	
	char* res = malloc(sizeof(char)*(l+1));
	
	sprintf(res, "%s%s", s1, s2);
	
	free(s1);
	if(v->memberAccess != NULL){ free(s2); }
	
	return res;
}

char* strSimpleVar(struct SimpleVar* s){
	
	//we approximate here, and could be wrong.
	//this is definitely not bulletproof.
	
	uint16_t l = DEFAULT_STR_SIZE+1;
	
	l += s->count_indices * (20+2);
	
	char* res = malloc(sizeof(char)*l);
	
	strcpy(res, s->name);
	
	for(uint16_t i = 0; i < s->count_indices; i++){
		
		char* s1 = strExpr(s->indices[i]);
		
		strcat(res, s1);
		
		free(s1);
	}
	
	return res;
}

char* strExpr(struct Expr* e){

	char* strTerm1 = strUnOpTerm(e->term1);
	
	char* strO = (e->op != NULL)?strOp(e->op):"";
	
	char* strTerm2 = (e->term2 != NULL)?strUnOpTerm(e->term2):"";
	
	uint16_t l1 = strlen(strTerm1);
	uint16_t l2 = strlen(strO);
	uint16_t l3 = strlen(strTerm2);
	
	uint16_t l = l1+l2+l3+1;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "%s%s%s", strTerm1, strO, strTerm2);
	
	free(strTerm1);
	if(e->op != NULL){ free(strO); }
	if(e->term2 != NULL){ free(strTerm2); }
	
	return res;
}

char* strOp(struct Op* o){
	
	char* res = malloc(sizeof(char)*6);
	
	sprintf(res, "%s", o->op);
	
	return res;
}

char* strUnOpTerm(struct UnOpTerm* u){

	char* strO = (u->op != NULL)?strOp(u->op):"";
	
	char* strT = strTerm(u->term);
	
	uint16_t l = strlen(strO) + strlen(strT)+3;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "%s%s", strO, strT);
	
	if(u->op != NULL){ free(strO); }
	free(strT);
	
	return res;
}

char* strTerm(struct Term* t){
	
	switch(t->kind){
		
		case 1: return strBoolConst(t->ptr.m1); 
		case 2: return strIntConst(t->ptr.m2);
		case 3: return strCharConst(t->ptr.m3);
		
		case 4: return strCall(t->ptr.m4);
		
		case 5: return strExpr(t->ptr.m5);
		case 6: return strVariable(t->ptr.m6);
		case 7: return strFloatConst(t->ptr.m7);
		case 8: return strStringConst(t->ptr.m8);
		case 9: return strHexConst(t->ptr.m9);
		
		case 10: return strBinConst(t->ptr.m10);
	}
	
	printf("[AST][Error] kind = %d\n", t->kind);
	exit(1);
	return NULL;
}

char* strAssignStmt(struct AssignStmt* a){
	
	char* strOptType = "";
	
	if(a->optType != NULL){
			
		strOptType = strType(a->optType);
	}
	
	char* strVar = strVariable(a->var);
	
	char* strE = strExpr(a->expr);
	
	uint16_t l1 = strlen(strOptType);
	uint16_t l2 = strlen(strVar);
	uint16_t l3 = strlen(strE);
	
	uint16_t l = l1+l2+l3+1+4;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "%s %s = %s", strOptType, strVar, strE);
	
	if(a->optType != NULL){ free(strOptType); }
	free(strVar);
	free(strE);
	
	return res;
}

char* strCall(struct Call* m){

	//we approximate here, and could be wrong.
	//this is definitely not bulletproof.
	
	uint16_t l = DEFAULT_STR_SIZE+1+2;
	
	l += m->count_args * (20+1);
	
	char* res = malloc(sizeof(char)*l);
	
	strcpy(res, m->name);
	strcat(res, "(");
	
	for(uint16_t i = 0; i < m->count_args; i++){
		
		char* s1 = strExpr(m->args[i]);
		
		strcat(res, s1);
		
		if(i != (m->count_args - 1)){
			strcat(res, ",");
		}
		
		free(s1);
	}
	
	strcat(res, ")");
	
	return res;
}

char* strRetStmt(struct RetStmt* r){
	
	char* s = strExpr(r->returnValue);
	
	uint16_t l = 10 + strlen(s);
	
	char* res =  malloc(sizeof(char)*l);
	
	sprintf(res, "return %s;", s);
	
	free(s);
	
	return res;
}
