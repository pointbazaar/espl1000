#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "ASTWriter.h"
#include "free_ast.h"
#include "copy_ast.h"
#include "ast_reader.h"
#include "ast.h"

//constants
void test_serialize_IntConst(bool debug);
void test_serialize_BoolConst(bool debug);
void test_serialize_TypeParam(bool debug);
void test_serialize_FloatConst(bool debug);

void test_serialize_Op(bool debug);
void test_serialize_Term(bool debug);
void test_serialize_Expr(bool debug);

FILE* file;

int main(){
	
	bool debug = false;
	
	file = fopen("/tmp/test", "w+");
	
	printf("[AST] running AST Reader/Writer Tests...\n");
	
	//test primitive serialization/deserialization
	test_serialize_IntConst(debug);
	test_serialize_BoolConst(debug);
	test_serialize_TypeParam(debug);
	test_serialize_FloatConst(debug);
	
	test_serialize_Op(debug);
	test_serialize_Term(debug);
	test_serialize_Expr(debug);
	
	printf("[AST] Passed All Tests\n");
	
	fclose(file);
	
	return 0;
}

void test_serialize_IntConst(bool debug){
	
	rewind(file);
	
	struct IntConst* m = malloc(sizeof(struct IntConst));
	m->value = 384;
	
	writeIntConst(m, file);
	
	rewind(file);
	
	struct IntConst* m2 = readIntConst(file, debug);
	
	assert(m->value == m2->value);
	
	freeIntConst(m);
	freeIntConst(m2);
}


void test_serialize_BoolConst(bool debug){
	
	rewind(file);
	
	struct BoolConst* m = malloc(sizeof(struct BoolConst));
	m->value = true;
	
	writeBoolConst(m, file);
	
	rewind(file);
	
	struct BoolConst* m2 = readBoolConst(file, debug);
	
	assert(m->value == m2->value);
	
	freeBoolConst(m);
	freeBoolConst(m2);
}

void test_serialize_TypeParam(bool debug){
	
	rewind(file);
	
	struct TypeParam* m = malloc(sizeof(struct TypeParam));
	m->index = 5;
	
	writeTypeParam(m, file);
	
	rewind(file);
	
	struct TypeParam* m2 = readTypeParam(file, debug);
	
	assert(m->index == m2->index);
	
	freeTypeParam(m);
	freeTypeParam(m2);
}

void test_serialize_FloatConst(bool debug){
	
	rewind(file);
	
	struct FloatConst* m = malloc(sizeof(struct FloatConst));
	m->value = 3.54;
	
	writeFloatConst(m, file);
	
	rewind(file);
	
	struct FloatConst* m2 = readFloatConst(file, debug);
	
	assert(m->value == m2->value);
	
	freeFloatConst(m);
	freeFloatConst(m2);
}

void test_serialize_Op(bool debug){
	
	rewind(file);
	
	struct Op* m = malloc(sizeof(struct Op));
	
	//otherwise valgrind will complain
	//about uninitialized bytes
	memset(m, 0, sizeof(struct Op));
	
	strcpy(m->op, "hi");
	
	writeOp(m, file);
	
	rewind(file);
	
	struct Op* m2 = readOp(file, debug);
	
	assert(strcmp(m->op, m2->op)==0);
	
	freeOp(m);
	freeOp(m2);
}

void test_serialize_Term(bool debug){
	
	rewind(file);
	
	struct IntConst* myIntConst = malloc(sizeof(struct IntConst));
	myIntConst->value = 48;
	
	struct Term* m = malloc(sizeof(struct Term));
	m->kind = 2;
	m->ptr.m2 = myIntConst;
	
	writeTerm(m, file);
	
	rewind(file);
	
	struct Term* m2 = readTerm(file, debug);
	
	assert(m->ptr.m2->value == m2->ptr.m2->value);
	
	freeTerm(m);
	freeTerm(m2);
}


void test_serialize_Expr(bool debug){
	
	rewind(file);
	
	struct IntConst* myIntConst = malloc(sizeof(struct IntConst));
	myIntConst->value = 4549;
	
	struct Term* m = malloc(sizeof(struct Term));
	m->kind = 2;
	m->ptr.m2 = myIntConst;
	
	struct Op* myop = malloc(sizeof(struct Op));
	
	//otherwise valgrind will complain
	//about uninitialized bytes
	memset(myop, 0, sizeof(struct Op));
	
	strcpy(myop->op, "hi");
	
	struct UnOpTerm* uop = malloc(sizeof(struct UnOpTerm));
	uop->term = m;
	uop->op = myop;
	
	struct Expr* expr = malloc(sizeof(struct Expr));
	
	expr->term1 = copyUnOpTerm(uop);
	expr->term2 = copyUnOpTerm(uop);
	expr->op = copyOp(myop);
	
	writeExpr(expr, file);
	
	rewind(file);
	
	struct Expr* expr2 = readExpr(file, debug);
	
	assert(expr->term1->term->ptr.m2->value == expr2->term1->term->ptr.m2->value);
	
	assert(strcmp(expr->op->op, expr2->op->op) == 0);
	
	freeExpr(expr);
	freeExpr(expr2);
}
