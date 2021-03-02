#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "../io/ast_writer.h"
#include "../util/free_ast.h"
#include "../util/copy_ast.h"
#include "../io/ast_reader.h"
#include "../ast.h"

//const
void test_serialize_IntConst(bool debug);
void test_serialize_HexConst(bool debug);
void test_serialize_BinConst(bool debug);
void test_serialize_BoolConst(bool debug);
void test_serialize_CharConst(bool debug);
void test_serialize_TypeParam(bool debug);
void test_serialize_FloatConst(bool debug);
void test_serialize_StringConst(bool debug);

void test_serialize_Op(bool debug);
void test_serialize_Term(bool debug);
void test_serialize_Expr(bool debug);

FILE* file;

int main(){
	
	bool debug = false;
	int count = 0;
	
	file = fopen("/tmp/test", "w+");
	
	printf("[AST Module] running AST Reader/Writer Tests...\n");
	
	//const
	test_serialize_BoolConst(debug);
	test_serialize_CharConst(debug);
	test_serialize_FloatConst(debug);
	test_serialize_IntConst(debug);
	test_serialize_HexConst(debug);
	test_serialize_BinConst(debug);
	test_serialize_StringConst(debug);
	count += 7;
	
	test_serialize_TypeParam(debug);
	count += 1;
	
	test_serialize_Op(debug);
	test_serialize_Term(debug);
	test_serialize_Expr(debug);
	count += 3;
	
	printf("[AST Module] Passed All (%d) Tests\n", count);
	
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

void test_serialize_HexConst(bool debug){
	
	rewind(file);
	
	struct HexConst* m = malloc(sizeof(struct HexConst));
	m->value = 0x48;
	
	writeHexConst(m, file);
	
	rewind(file);
	
	struct HexConst* m2 = readHexConst(file, debug);
	
	assert(m->value == m2->value);
	
	freeHexConst(m);
	freeHexConst(m2);
}

void test_serialize_BinConst(bool debug){
	
	rewind(file);
	
	struct BinConst* m = malloc(sizeof(struct BinConst));
	m->value = 0b00011101;
	
	writeBinConst(m, file);
	
	rewind(file);
	
	struct BinConst* m2 = readBinConst(file, debug);
	
	assert(m->value == m2->value);
	
	freeBinConst(m);
	freeBinConst(m2);
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

void test_serialize_CharConst(bool debug){
	
	rewind(file);
	
	struct CharConst* m = malloc(sizeof(struct CharConst));
	m->value = true;
	
	writeCharConst(m, file);
	
	rewind(file);
	
	struct CharConst* m2 = readCharConst(file, debug);
	
	assert(m->value == m2->value);
	
	freeCharConst(m);
	freeCharConst(m2);
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

void test_serialize_StringConst(bool debug){
	
	rewind(file);
	
	struct StringConst* m = malloc(sizeof(struct StringConst));
	char* str = malloc(sizeof(char)*100);
	strcpy(str, "hello");
	m->value = str;
	
	writeStringConst(m, file);
	
	rewind(file);
	
	struct StringConst* m2 = readStringConst(file, debug);
	
	assert(strcmp(m->value, m2->value) == 0);
	
	freeStringConst(m);
	freeStringConst(m2);
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
