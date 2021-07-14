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

#include "test_str_ast.h"

//const
void test_serialize_IntConst(bool debug);
void test_serialize_HexConst(bool debug);
void test_serialize_BinConst(bool debug);
void test_serialize_BoolConst(bool debug);
void test_serialize_CharConst(bool debug);
void test_serialize_FloatConst(bool debug);
void test_serialize_StringConst(bool debug);

//types
void test_serialize_TypeParam(bool debug);
void test_serialize_PrimitiveType(bool debug);

//expr
void test_serialize_Op(bool debug);
void test_serialize_Term(bool debug);
void test_serialize_Expr(bool debug);

FILE* file;

static void status(char* msg){

	printf(" - [TEST] %s\n", msg);
}

int main(){
	
	bool debug = false;
	
	file = fopen("/tmp/test", "w+");
	
	printf("[AST][TEST] running AST Reader/Writer Tests...\n");
	
	//const
	test_serialize_BoolConst(debug);
	test_serialize_CharConst(debug);
	test_serialize_FloatConst(debug);
	test_serialize_IntConst(debug);
	test_serialize_HexConst(debug);
	test_serialize_BinConst(debug);
	test_serialize_StringConst(debug);
	
	//types
	test_serialize_TypeParam(debug);
	test_serialize_PrimitiveType(debug);
	
	//expr
	test_serialize_Op(debug);
	test_serialize_Term(debug);
	test_serialize_Expr(debug);
	
	printf("[AST][TEST] running AST str Tests...\n");
	
	//--- from test_str_ast.c ---
	test_str_binconst();
	test_str_intconst();
	test_str_charconst();
	test_str_floatconst();
	test_str_hexconst();

	test_str_expr();
	test_str_op();
	test_str_unopterm();
	test_str_term();
	
	test_str_structdecl();
	test_str_structmember();
	
	test_str_lambda();
	
	printf("[AST Module] Passed All Tests\n");
	
	fclose(file);
	
	return 0;
}

void test_serialize_IntConst(bool debug){
	
	status("IntConst");
	
	rewind(file);
	
	struct IntConst m;
	m.value = 384;
	m.super.line_num = 33;
	
	writeIntConst(&m, file);
	
	rewind(file);
	
	struct IntConst* m2 = readIntConst(file, debug);
	
	assert(m.value == m2->value);
	assert(m.super.line_num == m2->super.line_num);
	
	freeIntConst(m2);
}

void test_serialize_HexConst(bool debug){
	
	status("HexConst");
	
	rewind(file);
	
	struct HexConst m;
	m.value = 0x48;
	m.super.line_num = 34;
	
	writeHexConst(&m, file);
	
	rewind(file);
	
	struct HexConst* m2 = readHexConst(file, debug);
	
	assert(m.value == m2->value);
	assert(m.super.line_num == m2->super.line_num);
	
	freeHexConst(m2);
}

void test_serialize_BinConst(bool debug){
	
	status("BinConst");
	
	rewind(file);
	
	struct BinConst m;
	m.value = 0b00011101;
	m.super.line_num = 78;
	
	writeBinConst(&m, file);
	
	rewind(file);
	
	struct BinConst* m2 = readBinConst(file, debug);
	
	assert(m.value == m2->value);
	assert(m.super.line_num == m2->super.line_num);
	
	freeBinConst(m2);
}

void test_serialize_BoolConst(bool debug){
	
	status("BoolConst");
	
	rewind(file);
	
	struct BoolConst m;
	m.value = true;
	
	writeBoolConst(&m, file);
	
	rewind(file);
	
	struct BoolConst* m2 = readBoolConst(file, debug);
	
	assert(m.value == m2->value);
	
	freeBoolConst(m2);
}

void test_serialize_CharConst(bool debug){
	
	status("CharConst");
	
	rewind(file);
	
	struct CharConst m;
	m.value = true;
	
	writeCharConst(&m, file);
	
	rewind(file);
	
	struct CharConst* m2 = readCharConst(file, debug);
	
	assert(m.value == m2->value);
	
	freeCharConst(m2);
}

void test_serialize_TypeParam(bool debug){
	
	status("TypeParam");
	
	rewind(file);
	
	struct TypeParam m;
	m.index = 5;
	
	writeTypeParam(&m, file);
	
	rewind(file);
	
	struct TypeParam* m2 = readTypeParam(file, debug);
	
	assert(m.index == m2->index);
	
	freeTypeParam(m2);
}

void test_serialize_PrimitiveType(bool debug){

	status("PrimitiveType");

	rewind(file);
	
	struct PrimitiveType p = {
		.is_int_type   = true,
		.is_float_type = false,
		.is_char_type  = false,
		.is_bool_type  = false,
		.int_type = INT
	};
	
	writePrimitiveType(&p, file);
	
	rewind(file);
	
	struct PrimitiveType* p2 = readPrimitiveType(file, debug);
	
	assert(p2 != NULL);
	
	assert(p2->is_int_type == true);
	assert(p2->is_float_type == false);
	assert(p2->is_char_type == false);
	assert(p2->is_bool_type == false);
	assert(p2->int_type == INT);
	
	freePrimitiveType(p2);
}

void test_serialize_FloatConst(bool debug){
	
	status("FloatConst");
	
	rewind(file);
	
	struct FloatConst m = { .value = 3.54 };
	
	writeFloatConst(&m, file);
	
	rewind(file);
	
	struct FloatConst* m2 = readFloatConst(file, debug);
	
	assert(m.value == m2->value);
	
	freeFloatConst(m2);
}

void test_serialize_StringConst(bool debug){
	
	status("StringConst");
	
	rewind(file);
	
	struct StringConst m;
	
	char* str = malloc(sizeof(char)*100);
	strcpy(str, "hello");
	m.value = str;
	
	writeStringConst(&m, file);
	
	rewind(file);
	
	struct StringConst* m2 = readStringConst(file, debug);
	
	assert(strcmp(m.value, m2->value) == 0);
	
	freeStringConst(m2);
}

void test_serialize_Op(bool debug){
	
	status("Op");
	
	rewind(file);
	
	struct Op m;
	
	//otherwise valgrind will complain
	//about uninitialized bytes
	memset(&m, 0, sizeof(struct Op));
	
	strcpy(m.op, "hi");
	
	writeOp(&m, file);
	
	rewind(file);
	
	struct Op* m2 = readOp(file, debug);
	
	assert(strcmp(m.op, m2->op)==0);
	
	freeOp(m2);
}

void test_serialize_Term(bool debug){
	
	status("Term");
	
	rewind(file);
	
	struct IntConst myIntConst = { .value = 48 };
	
	struct Term m = {
		.kind = 2,
		.ptr.m2 = &myIntConst
	};
	
	writeTerm(&m, file);
	
	rewind(file);
	
	struct Term* m2 = readTerm(file, debug);
	
	assert(m.ptr.m2->value == m2->ptr.m2->value);
	
	freeTerm(m2);
}

void test_serialize_Expr(bool debug){
	
	status("Expr");
	
	rewind(file);
	
	struct IntConst myIntConst = {
		.super = {
			.line_num = 33,
			.annotations = 0
		},
		.value = 4549
	};
	
	struct Term m = {
		.kind = 2,
		.ptr.m2 = &myIntConst
	};	
	
	struct Op myop;
	
	//otherwise valgrind will complain
	//about uninitialized bytes
	memset(&myop, 0, sizeof(struct Op));
	
	strcpy(myop.op, "hi");
	
	struct UnOpTerm uop = {
		.term = &m,
		.op   = &myop
	};
	
	struct Expr expr = {
		.term1 = copyUnOpTerm(&uop),
		.term2 = copyUnOpTerm(&uop),
		.op    = copyOp(&myop)
	};
	
	writeExpr(&expr, file);
	
	rewind(file);
	
	struct Expr* expr2 = readExpr(file, debug);
	
	assert(expr.term1->term->ptr.m2->value == expr2->term1->term->ptr.m2->value);
	
	assert(strcmp(expr.op->op, expr2->op->op) == 0);
	
	freeExpr(expr2);
}
