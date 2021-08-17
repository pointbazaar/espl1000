#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "../io/ast_writer.h"
#include "../util/free_ast.h"
#include "../util/copy_ast.h"
#include "../io/ast_reader.h"

#include "test_str_ast.h"

//const
void test_serialize_int_const();
void test_serialize_hex_const();
void test_serialize_bin_const();
void test_serialize_bool_const();
void test_serialize_char_const();
void test_serialize_float_const();
void test_serialize_string_const();

//c_types_util
void test_serialize_type_param();
void test_serialize_primitive_type();

//expr
void test_serialize_op();
void test_serialize_term();
void test_serialize_expr();

FILE* file;

static void test_status_ast(char* msg){

	printf(" - [TEST] %s\n", msg);
}

int main(){

	file = fopen("/tmp/test", "w+");
	
	printf("[AST][TEST] running AST Reader/Writer Tests...\n");
	
	//const
	test_serialize_bool_const();
	test_serialize_char_const();
	test_serialize_float_const();
	test_serialize_int_const();
	test_serialize_hex_const();
	test_serialize_bin_const();
	test_serialize_string_const();
	
	//c_types_util
	test_serialize_type_param();
	test_serialize_primitive_type();
	
	//expr
	test_serialize_op();
	test_serialize_term();
	test_serialize_expr();
	
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
	
	test_str_lambda();
	
	printf("[AST Module] Passed All Tests\n");
	
	fclose(file);
	
	return 0;
}

void test_serialize_int_const() {

	test_status_ast("IntConst");
	
	rewind(file);
	
	struct IntConst m;
	m.value = 384;
	m.super.line_num = 33;

	write_int_const(&m, file);
	
	rewind(file);
	
	struct IntConst* m2 = read_int_const(file);
	
	assert(m.value == m2->value);
	assert(m.super.line_num == m2->super.line_num);

	free_int_const(m2);
}

void test_serialize_hex_const() {

	test_status_ast("HexConst");
	
	rewind(file);
	
	struct HexConst m;
	m.value = 0x48;
	m.super.line_num = 34;

	write_hex_const(&m, file);
	
	rewind(file);
	
	struct HexConst* m2 = read_hex_const(file);
	
	assert(m.value == m2->value);
	assert(m.super.line_num == m2->super.line_num);

	free_hex_const(m2);
}

void test_serialize_bin_const() {

	test_status_ast("BinConst");
	
	rewind(file);
	
	struct BinConst m;
	m.value = 0b00011101;
	m.super.line_num = 78;

	write_bin_const(&m, file);
	
	rewind(file);
	
	struct BinConst* m2 = read_bin_const(file);
	
	assert(m.value == m2->value);
	assert(m.super.line_num == m2->super.line_num);

	free_bin_const(m2);
}

void test_serialize_bool_const() {

	test_status_ast("BoolConst");
	
	rewind(file);
	
	struct BoolConst m;
	m.value = true;

	write_bool_const(&m, file);
	
	rewind(file);
	
	struct BoolConst* m2 = read_bool_const(file);
	
	assert(m.value == m2->value);

	free_bool_const(m2);
}

void test_serialize_char_const() {

	test_status_ast("CharConst");
	
	rewind(file);
	
	struct CharConst m;
	m.value = true;

	write_char_const(&m, file);
	
	rewind(file);
	
	struct CharConst* m2 = read_char_const(file);
	
	assert(m.value == m2->value);

	free_char_const(m2);
}

void test_serialize_type_param() {

	test_status_ast("TypeParam");
	
	rewind(file);
	
	struct TypeParam m;
	m.index = 5;

	write_type_param(&m, file);
	
	rewind(file);
	
	struct TypeParam* m2 = read_type_param(file);
	
	assert(m.index == m2->index);

	free_type_param(m2);
}

void test_serialize_primitive_type() {

	test_status_ast("PrimitiveType");

	rewind(file);
	
	struct PrimitiveType p = {
		.is_int_type   = true,
		.is_float_type = false,
		.is_char_type  = false,
		.is_bool_type  = false,
		.int_type = INT
	};

	write_primitive_type(&p, file);
	
	rewind(file);
	
	struct PrimitiveType* p2 = read_primitive_type(file);
	
	assert(p2 != NULL);
	
	assert(p2->is_int_type == true);
	assert(p2->is_float_type == false);
	assert(p2->is_char_type == false);
	assert(p2->is_bool_type == false);
	assert(p2->int_type == INT);

	free_primitive_type(p2);
}

void test_serialize_float_const() {

	test_status_ast("FloatConst");
	
	rewind(file);
	
	struct FloatConst m = { .value = 3.54f };

	write_float_const(&m, file);
	
	rewind(file);
	
	struct FloatConst* m2 = read_float_const(file);
	
	assert(m.value == m2->value);

	free_float_const(m2);
}

void test_serialize_string_const() {

	test_status_ast("StringConst");
	
	rewind(file);
	
	struct StringConst m;
	
	char* str = malloc(sizeof(char)*100);
	strcpy(str, "hello");
	m.value = str;

	write_string_const(&m, file);
	
	rewind(file);
	
	struct StringConst* m2 = read_string_const(file);
	
	assert(strcmp(m.value, m2->value) == 0);

	free_string_const(m2);
}

void test_serialize_op() {

	test_status_ast("Op");
	
	rewind(file);
	
	struct Op m;
	
	//otherwise valgrind will complain
	//about uninitialized bytes
	memset(&m, 0, sizeof(struct Op));
	
	strcpy(m.op, "hi");

	write_op(&m, file);
	
	rewind(file);
	
	struct Op* m2 = read_op(file);
	
	assert(strcmp(m.op, m2->op)==0);

	free_op(m2);
}

void test_serialize_term() {

	test_status_ast("Term");
	
	rewind(file);
	
	struct IntConst myIntConst = { .value = 48 };

	struct ConstValue mcv = {.kind = 2, .ptr.m2_int_const = &myIntConst};

	struct Term m = {
		.kind = 12,
		.ptr.m12 = &mcv
	};

	write_term(&m, file);
	
	rewind(file);
	
	struct Term* m2 = read_term(file);
	
	assert(m.ptr.m12->ptr.m2_int_const->value == m2->ptr.m12->ptr.m2_int_const->value);

	free_term(m2);
}

void test_serialize_expr() {

	test_status_ast("Expr");
	
	rewind(file);
	
	struct IntConst myIntConst = {
		.super = {
			.line_num = 33,
			.annotations = 0
		},
		.value = 4549
	};

	struct ConstValue mcv = {
			.kind = 2,
			.ptr.m2_int_const = &myIntConst
	};
	
	struct Term m = {
		.kind = 12,
		.ptr.m12 = &mcv
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
		.term1 = copy_un_op_term(&uop),
		.term2 = copy_un_op_term(&uop),
		.op    = copy_op(&myop)
	};

	write_expr(&expr, file);
	
	rewind(file);
	
	struct Expr* expr2 = read_expr(file);
	
	assert(expr.term1->term->ptr.m12->ptr.m2_int_const->value == expr2->term1->term->ptr.m12->ptr.m2_int_const->value);
	
	assert(strcmp(expr.op->op, expr2->op->op) == 0);

	free_expr(expr2);
}
