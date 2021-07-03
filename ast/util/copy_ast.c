#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../ast.h"
#include "copy_ast.h"

struct BoolConst* copyBoolConst(struct BoolConst* bc){
	
	struct BoolConst* res = make(BoolConst);
	res->value = bc->value;
	return res;
}

struct CharConst* copyCharConst(struct CharConst* cc){
	
	struct CharConst* res = make(CharConst);
	res->value = cc->value;
	return res;
}

struct Expr* copyExpr(struct Expr* expr){
	
	struct Expr* res = make(Expr);
	res->op = NULL;
	res->term2 = NULL;
	
	res->term1 = copyUnOpTerm(expr->term1);
	if(res->op != NULL){
		res->op = copyOp(expr->op);
		res->term2 = copyUnOpTerm(expr->term2);
	}
	return res;
}

struct FloatConst* copyFloatConst(struct FloatConst* fc){
	
	struct FloatConst* res = make(FloatConst);
	res->value = fc->value;
	return res;
}

struct Identifier* copyIdentifier(struct Identifier* id){
	
	struct Identifier* res = make(Identifier);
	strcpy(res->identifier, id->identifier);
	return res;
}

struct Lambda* copyLambda(struct Lambda* l){

	struct Lambda* res = make(Lambda);
	res->count_params = l->count_params;
	for(uint8_t i = 0; i < l->count_params; i++){
		res->params[i] = copyIdentifier(l->params[i]);
	}
	
	res->expr = copyExpr(l->expr);
	return res;
}

struct IntConst* copyIntConst(struct IntConst* ic){
	
	struct IntConst* res = make(IntConst);
	res->value = ic->value;
	return res;
}

struct HexConst* copyHexConst(struct HexConst* hc){
	
	struct HexConst* res = make(HexConst);
	res->value = hc->value;
	return res;
}

struct BinConst* copyBinConst(struct BinConst* hc){
	
	struct BinConst* res = make(BinConst);
	res->value = hc->value;
	return res;
}

struct SimpleVar* copySimpleVar(struct SimpleVar* sv){
	
	struct SimpleVar* res = make(SimpleVar);
	
	strcpy(res->name, sv->name);
	res->count_indices = sv->count_indices;
	res->indices = malloc(sizeof(struct Expr*)*res->count_indices);
	
	for(int i = 0;i < res->count_indices; i++){
		res->indices[i] = copyExpr(sv->indices[i]);
	}
	
	return res;
}

struct Term* copyTerm(struct Term* t){
	
	struct Term* res = make(Term);
	res->kind = t->kind;
	
	switch(t->kind){
		
		case  1: res->ptr.m1 = copyBoolConst(t->ptr.m1);   break;
		case  2: res->ptr.m2 = copyIntConst(t->ptr.m2);    break;
		case  3: res->ptr.m3 = copyCharConst(t->ptr.m3);   break;
		case  4: res->ptr.m4 = copyCall(t->ptr.m4);        break;
		case  5: res->ptr.m5 = copyExpr(t->ptr.m5);        break;
		case  6: res->ptr.m6 = copyVariable(t->ptr.m6);    break;
		case  7: res->ptr.m7 = copyFloatConst(t->ptr.m7);  break;
		case  8: res->ptr.m8 = copyStringConst(t->ptr.m8); break;
		case  9: res->ptr.m9 = copyHexConst(t->ptr.m9);    break;
		case 10: res->ptr.m10 = copyBinConst(t->ptr.m10);  break;
		default:
			printf("[AST][Error] copyTerm(...)\n");
			exit(1);
	}
	
	return res;
}

struct UnOpTerm* copyUnOpTerm(struct UnOpTerm* t){
	
	struct UnOpTerm* res = make(UnOpTerm);
	
	res->op = NULL;
	
	if(t->op != NULL){ res->op = copyOp(t->op); }
	
	res->term = copyTerm(t->term);
	
	return res;
}

struct Variable* copyVariable(struct Variable* var){

	struct Variable* res = make(Variable);
	
	res->simpleVar = copySimpleVar(var->simpleVar);

	res->memberAccess = NULL;

	if(var->memberAccess != NULL){

		res->memberAccess = copyVariable(var->memberAccess);
	}
	
	return res;
}

struct DeclArg* copyDeclArg(struct DeclArg* d){

	struct DeclArg* res = make(DeclArg);
	
	res->type = copyType(d->type);
	res->has_name = d->has_name;
	strcpy(res->name, d->name);
	
	return res;
}

struct Op* copyOp(struct Op* op){
	
	struct Op* res = make(Op);
	strcpy(res->op, op->op);
	return res;
}

struct StringConst* copyStringConst(struct StringConst* s){
	
	struct StringConst* res = make(StringConst);
	res->value = malloc(sizeof(char)*(strlen(s->value)+1));
	strcpy(res->value, s->value);
	return res;
}

struct Type* copyType(struct Type* t){
	
	struct Type* res = make(Type);
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	
	if(t->m1 != NULL){ res->m1 = copyBasicType(t->m1); }
	if(t->m2 != NULL){ res->m2 = copyTypeParam(t->m2); }
	if(t->m3 != NULL){ res->m3 = copyArrayType(t->m3); }
	
	return res;
}

struct TypeParam* copyTypeParam(struct TypeParam* t){
	
	struct TypeParam* res = make(TypeParam);
	
	res->index = t->index;
	
	return res;
}

struct SubrType* copySubrType(struct SubrType* s){
	
	struct SubrType* res = make(SubrType);
	
	res->hasSideEffects = s->hasSideEffects;
	res->returnType = copyType(s->returnType);
	
	res->count_argTypes = s->count_argTypes;
	
	res->argTypes = malloc(sizeof(struct Type*)*(s->count_argTypes));
	
	for(uint8_t i = 0; i < res->count_argTypes; i++){
	
		res->argTypes[i] = copyType(s->argTypes[i]);
	}
	
	return res;
}

struct SimpleType* copySimpleType(struct SimpleType* s){
	
	struct SimpleType* res = make(SimpleType);
	
	res->primitiveType = NULL;
	res->structType    = NULL;
	
	if(s->primitiveType != NULL){ res->primitiveType = copyPrimitiveType(s->primitiveType); }
	if(s->structType    != NULL){ res->structType    = copyStructType(s->structType); }
	
	return res;
}

struct StructType* copyStructType(struct StructType* s){
	
	struct StructType* res = make(StructType);
	
	strcpy(res->typeName, s->typeName);
	
	res->typeParamCount = s->typeParamCount;
	
	res->typeParams = malloc(sizeof(uint8_t)*(s->typeParamCount));
	
	memcpy(res->typeParams, s->typeParams, sizeof(uint8_t)*(s->typeParamCount));
	
	return res;
}

struct PrimitiveType* copyPrimitiveType(struct PrimitiveType* p){
	
	struct PrimitiveType* res = make(PrimitiveType);
	
	memcpy(res, p, sizeof(struct PrimitiveType));
	
	return res;
	
}

struct BasicType* copyBasicType(struct BasicType* b){

	struct BasicType* res = make(BasicType);
	
	res->simpleType = NULL;
	res->subrType   = NULL;
	
	if(b->simpleType != NULL){ res->simpleType = copySimpleType(b->simpleType); }
	if(b->subrType   != NULL){ res->subrType   = copySubrType(b->subrType); }
	
	return res;
}

struct ArrayType* copyArrayType(struct ArrayType* a){
	
	struct ArrayType* res = make(ArrayType);
	
	res->element_type = copyType(a->element_type);
	
	return res;
}

struct Call* copyCall(struct Call* c){
	
	struct Call* res = make(Call);
	strcpy(res->name, c->name);
	
	res->args = malloc(sizeof(struct Expr*)*c->count_args);
	res->count_args = c->count_args;
	for(int i = 0;i < c->count_args;i++){
		res->args[i] = copyExpr(c->args[i]);
	}
	
	return res;
}
