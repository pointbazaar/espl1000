#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "str_ast.h"

static void error(char* msg){

	printf("[AST][Error] %s\n", msg);
	exit(1);
}

//---------------------------------------

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

char* strRange(struct Range* r){
	
	char* s1 = strExpr(r->start);
	char* s2 = strExpr(r->end);
	
	uint16_t l = strlen(s1) + strlen(s2) + 4+1;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "%s .. %s", s1, s2);
	
	free(s1); free(s2);
	
	return res;
}

char* strStmtBlock(struct StmtBlock* block){
	
	uint16_t l = 0;

	for(uint16_t i = 0; i < block->count; i++){
		
		char* s = strStmt(block->stmts[i]);
		
		l += strlen(s) + 1;
		
		free(s);
	}
	
	l += 1 + 2 + 2;
	
	char* res = malloc(sizeof(char)*l);
	
	strcpy(res, "");
	strcat(res, "{\n");
	
	for(uint16_t i = 0; i < block->count; i++){
		
		char* s = strStmt(block->stmts[i]);
		
		strcat(res, s);
		strcat(res, "\n");
		
		free(s);
	}
	strcat(res, "}\n");
	return res;
}

char* strLambda(struct Lambda* lambda){
	
	uint16_t l = 0;

	for(uint16_t i = 0; i < lambda->count_params; i++){
		
		char* s = strIdentifier(lambda->params[i]);
		l += strlen(s) + 1;
		free(s);
	}
	
	char* s2 = strExpr(lambda->expr);
	l += strlen(s2);
	
	l += 2 + 4;
	
	char* res = malloc(sizeof(char)*l);
	
	strcpy(res, "");
	strcat(res, "(");
	
	for(uint16_t i = 0; i < lambda->count_params; i++){
		
		char* s = strIdentifier(lambda->params[i]);
		strcat(res, s);
		
		if(i < (lambda->count_params - 1)){
			strcat(res, ",");
		}
		
		free(s);
	}
	strcat(res, ") -> ");
	strcat(res, s2);
	free(s2);
	
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
	sprintf(res, "%s", s->value);
	return res;
}

char* strBinConst(struct BinConst* b){
	
	char* res = malloc(sizeof(char)*129);
	
	uint32_t value = b->value;
	
	if(value == 0){
		sprintf(res, "0b0");
		return res;
	}
	
	const int size = 128;
	
	char buffer[size];
	
	int index = size - 1;
	buffer[index--] = '\0';
	
	while(value > 0){
		
		uint8_t bit = value & 0x1;
		
		buffer[index--] = (bit == 0x1) ? '1' : '0';
		
		value >>= 1;
	}

	sprintf(res, "0b%s", buffer+index+1);
	return res;
}

char* strArrayType(struct ArrayType* at){

	char* inner = strType(at->element_type);
	
	char* res = malloc(sizeof(char)*(strlen(inner)+2+1));
	
	sprintf(res, "[%s]", inner);
	
	free(inner);
	
	return res;
}

char* strBasicType(struct BasicType* btw){
	
	if(btw->simpleType != NULL){
		return strSimpleType(btw->simpleType);
	}
	
	if(btw->subrType != NULL){
		return strSubrType(btw->subrType);
	}
	
	error("strBasicTypeWrapped");
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
	
	if(t->m1 != NULL){ return strBasicType(t->m1); }
	
	if(t->m2 != NULL){ return strTypeParam(t->m2); }
	
	if(t->m3 != NULL){ return strArrayType(t->m3); }
	
	error("strType");
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
	
		error("strStructType");
	}
	
	char* res = malloc(DEFAULT_STR_SIZE);
	
	sprintf(res, "%s", s->typeName);
	
	return res;
}

char* strStructDecl(struct StructDecl* s){
	
	char* name = strSimpleType(s->type);

	uint16_t l = strlen("struct   { } ")+strlen(name);
	
	char* memberStrs[s->count_members];
	
	for(uint16_t i = 0; i < s->count_members; i++){
		
		char* s2 = strStructMember(s->members[i]);
		l += strlen(s2);
		
		memberStrs[i] = s2;
	}
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "struct %s {", name);
	
	for(uint16_t i = 0; i < s->count_members; i++){
		
		char* s = memberStrs[i];
		strcat(res, s);
		free(s);
	}
	
	strcat(res, "}");
	
	return res;
}

char* strStructMember(struct StructMember* s){

	char* s1 = strType(s->type);
	const int l = strlen(s1);
	
	char* res = malloc(sizeof(char)*(l+DEFAULT_STR_SIZE+3));
	
	sprintf(res, "%s %s;", s1, s->name);
	
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
	//TODO: replace with a robust implementation
	
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
		
		case 1: return strBoolConst  (t->ptr.m1); 
		case 2: return strIntConst   (t->ptr.m2);
		case 3: return strCharConst  (t->ptr.m3);
		case 4: return strCall       (t->ptr.m4);
		case 5: return strExpr       (t->ptr.m5);
		case 6: return strVariable   (t->ptr.m6);
		case 7: return strFloatConst (t->ptr.m7);
		case 8: return strStringConst(t->ptr.m8);
		case 9: return strHexConst   (t->ptr.m9);
		
		case 10: return strBinConst  (t->ptr.m10);
	}
	
	error("strTerm");
	return NULL;
}

char* strStmt(struct Stmt* stmt){
	
	switch(stmt->kind){
		
		case 0: return strLoopStmt  (stmt->ptr.m0);
		case 1: return strCall      (stmt->ptr.m1);
		case 2: return strWhileStmt (stmt->ptr.m2);
		case 3: return strIfStmt    (stmt->ptr.m3);
		case 4: return strRetStmt   (stmt->ptr.m4);
		case 5: return strAssignStmt(stmt->ptr.m5);
		
		case 6: return strForStmt   (stmt->ptr.m7);
		case 7: return strSwitchStmt(stmt->ptr.m8);
		
		case 99: {
			//break,continue,...
			char* res = malloc(sizeof(char)*30);
			strcpy(res, "");
			
			if(stmt->isBreak){
				sprintf(res, "break");
			}
			if(stmt->isContinue){
				sprintf(res, "continue");
			}
			return res;
		}
			
		default:
			error("strStmt");
			return NULL;
	}
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

char* strForStmt(struct ForStmt* f){
	
	char* s1 = strRange(f->range);
	char* s2 = strStmtBlock(f->block);
	
	const uint32_t l = strlen(s1)+strlen(s2)
				+3+4+1+DEFAULT_STR_SIZE;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "for %s in %s %s", f->indexName, s1, s2);
	
	free(s1); free(s2);
	return res;
}

char* strIfStmt(struct IfStmt* i){

	char* s1 = strExpr(i->condition);
	char* s2 = strStmtBlock(i->block);
	
	char* s3 = "";
	
	if(i->elseBlock != NULL){
		s3 = strStmtBlock(i->elseBlock);
	}
	
	const uint32_t l = strlen(s1)+strlen(s2)+strlen(s3)
		+1+10;
		
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "if %s %s", s1, s2);
	
	free(s1); free(s2);
	
	if(i->elseBlock != NULL){
		
		strcat(res, " else ");
		strcat(res, s3);
		free(s3);
	}
	
	return res;
}

char* strLoopStmt(struct LoopStmt* l){
	
	char* s1 = strExpr(l->count);
	char* s2 = strStmtBlock(l->block);
	
	const uint32_t l1 = strlen(s1)+strlen(s2)+4+2+1;
	
	char* res = malloc(sizeof(char)*l1);
	
	sprintf(res, "loop %s %s", s1, s2);
	
	free(s1); free(s2);
	return res;
}

char* strWhileStmt(struct WhileStmt* w){

	char* s1 = strExpr(w->condition);
	char* s2 = strStmtBlock(w->block);
	
	const uint32_t l = strlen(s1)+strlen(s2)+5+2+1;
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "while %s %s", s1, s2);
	
	free(s1); free(s2);
	return res;
}

char* strSwitchStmt(struct SwitchStmt* s){

	uint16_t l = strlen("switch { } ");
	
	char* s1 = strExpr(s->expr);
	l += strlen(s1);
	
	char* strCases[s->count_cases];
	
	for(uint16_t i = 0; i < s->count_cases; i++){
		
		strCases[i] = strCaseStmt(s->cases[i]);
		l += strlen(strCases[i]);
	}
	
	char* res = malloc(sizeof(char)*l);
	
	sprintf(res, "switch %s {", s1);
	
	free(s1);
	
	for(uint16_t i = 0; i < s->count_cases; i++){
		
		strcat(res, strCases[i]);
	}
	
	strcat(res, "}");
	
	return res;
}

char* strCall(struct Call* m){

	//we approximate here, and could be wrong.
	//this is definitely not bulletproof.
	//TODO: replace with more robust implementation
	
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

char* strCaseStmt(struct CaseStmt* c){
	
	char* s = NULL;
	
	switch(c->kind){
	
		case 0: s = strBoolConst (c->ptr.m1); break;
		case 1: s = strCharConst (c->ptr.m2); break;
		case 2: s = strIntConst  (c->ptr.m3); break;
	}
	
	char* s2 = strStmtBlock(c->block);
	
	uint16_t l = strlen(s) + strlen(s2);

	char* res = malloc(sizeof(char)*(l+1+6));
	
	sprintf(res, "case %s %s", s, s2);
	
	free(s); free(s2);
	
	return res;
}
