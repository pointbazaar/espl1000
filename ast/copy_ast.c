#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "copy_ast.h"

struct BoolConst* copyBoolConst(struct BoolConst* bc){
	
	struct BoolConst* res = malloc(sizeof(struct BoolConst));
	res->value = bc->value;
	return res;
}	

struct CharConst* copyCharConst(struct CharConst* cc){
	struct CharConst* res = malloc(sizeof(struct CharConst));
	res->value = cc->value;
	return res;
}

struct Expr* copyExpr(struct Expr* expr){
	struct Expr* res = malloc(sizeof(struct Expr));
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
	struct FloatConst* res = malloc(sizeof(struct FloatConst));
	res->value = fc->value;
	return res;
}

struct Identifier* copyIdentifier(struct Identifier* id){
	struct Identifier* res = malloc(sizeof(struct Identifier));
	strcpy(res->identifier, id->identifier);
	return res;
}

struct IntConst* copyIntConst(struct IntConst* ic){
	struct IntConst* res = malloc(sizeof(struct IntConst));
	res->value = ic->value;
	return res;
}

struct HexConst* copyHexConst(struct HexConst* hc){
	struct HexConst* res = malloc(sizeof(struct HexConst));
	res->value = hc->value;
	return res;
}

struct BinConst* copyBinConst(struct BinConst* hc){
	struct BinConst* res = malloc(sizeof(struct BinConst));
	res->value = hc->value;
	return res;
}

struct SimpleVar* copySimpleVar(struct SimpleVar* sv){
	struct SimpleVar* res = malloc(sizeof(struct SimpleVar));
	
	strcpy(res->name, sv->name);
	res->count_indices = sv->count_indices;
	res->indices = malloc(sizeof(struct Expr*)*res->count_indices);
	
	for(int i=0;i < res->count_indices; i++){
		res->indices[i] = copyExpr(sv->indices[i]);
	}
	
	return res;
}

struct Term* copyTerm(struct Term* t){
	struct Term* res = malloc(sizeof(struct Term));
	res->kind = t->kind;
	
	switch(t->kind){
		case 1:
			res->ptr.m1 = copyBoolConst(t->ptr.m1);
			break;
		case 2:
			res->ptr.m2 = copyIntConst(t->ptr.m2);
			break;
		case 3:
			res->ptr.m3 = copyCharConst(t->ptr.m3);
			break;
		case 4:
			res->ptr.m4 = copyMethodCall(t->ptr.m4);
			break;
		case 5:
			res->ptr.m5 = copyExpr(t->ptr.m5);
			break;
		case 6:
			res->ptr.m6 = copyVariable(t->ptr.m6);
			break;
		case 7:
			res->ptr.m7 = copyFloatConst(t->ptr.m7);
			break;
		case 8:
			res->ptr.m8 = copyStringConst(t->ptr.m8);
			break;
		case 9:
			res->ptr.m9 = copyHexConst(t->ptr.m9);
			break;
		case 10:
			res->ptr.m10 = copyBinConst(t->ptr.m10);
			break;
		default:
		printf("Error in copyTerm(...)\n");
		exit(1);
	}
	
	return res;
}

struct UnOpTerm* copyUnOpTerm(struct UnOpTerm* t){
	struct UnOpTerm* res = malloc(sizeof(struct UnOpTerm));
	if(t->op != NULL){
		res->op = copyOp(t->op);
	}else{
		res->op = NULL;
	}
	res->term = copyTerm(t->term);
	return res;
}

struct Variable* copyVariable(struct Variable* var){

	struct Variable* res = malloc(sizeof(struct Variable));
	
	res->simpleVar = copySimpleVar(var->simpleVar);

	res->memberAccess = NULL;

	if(var->memberAccess != NULL){

		res->memberAccess = copyVariable(var->memberAccess);
	}
	
	return res;
}

struct Op* copyOp(struct Op* op){
	struct Op* res = malloc(sizeof(struct Op));
	strcpy(res->op, op->op);
	return res;
}

struct StringConst* copyStringConst(struct StringConst* s){
	struct StringConst* res = malloc(sizeof(struct StringConst));
	res->value = malloc(sizeof(char)*(strlen(s->value)+1));
	strcpy(res->value, s->value);
	return res;
}

struct MethodCall* copyMethodCall(struct MethodCall* c){
	struct MethodCall* res = malloc(sizeof(struct MethodCall));
	strcpy(res->methodName, c->methodName);
	
	res->args = malloc(sizeof(struct Expr*)*c->count_args);
	res->count_args = c->count_args;
	for(int i=0;i < c->count_args;i++){
		res->args[i] = copyExpr(c->args[i]);
	}
	
	return res;
}
