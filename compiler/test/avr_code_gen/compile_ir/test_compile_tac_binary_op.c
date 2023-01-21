#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "avr_code_gen/cg_avr.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_avr_code_gen.h"
#include "../test_avr_code_gen_util.h"

#include "test_compile_tac.h"

static void test_compile_tac_binary_op_add_8bit();
static void test_compile_tac_binary_op_add_16bit();
static void test_compile_tac_binary_op_sub_8bit();
static void test_compile_tac_binary_op_sub_16bit();
static void test_compile_tac_binary_op_and_8bit();
static void test_compile_tac_binary_op_and_16bit();
static void test_compile_tac_binary_op_or_8bit();
static void test_compile_tac_binary_op_or_16bit();
static void test_compile_tac_binary_op_xor_8bit();
static void test_compile_tac_binary_op_xor_16bit();

static void test_compile_tac_binary_op_neq_true_8bit();
static void test_compile_tac_binary_op_neq_true_16bit();

static void test_compile_tac_binary_op_neq_false_8bit();
static void test_compile_tac_binary_op_neq_false_16bit();

static void test_compile_tac_binary_op_lt_true_8bit();
static void test_compile_tac_binary_op_lt_true_16bit();

static void test_compile_tac_binary_op_lt_false_8bit();
static void test_compile_tac_binary_op_lt_false_16bit();

static void test_compile_tac_binary_op_eq_true_8bit();
static void test_compile_tac_binary_op_eq_true_16bit();

static void test_compile_tac_binary_op_eq_false_8bit();
static void test_compile_tac_binary_op_eq_false_16bit();

static int16_t make16(uint8_t high, uint8_t low){
	return (high << 8) | low;
}

void test_compile_tac_binary_op(){

	test_compile_tac_binary_op_add_8bit();
	test_compile_tac_binary_op_add_16bit();

	test_compile_tac_binary_op_sub_8bit();
	test_compile_tac_binary_op_sub_16bit();

	test_compile_tac_binary_op_and_8bit();
	test_compile_tac_binary_op_and_16bit();

	test_compile_tac_binary_op_or_8bit();
	test_compile_tac_binary_op_or_16bit();

	test_compile_tac_binary_op_xor_8bit();
	test_compile_tac_binary_op_xor_16bit();

	test_compile_tac_binary_op_neq_true_8bit();
	test_compile_tac_binary_op_neq_true_16bit();

	test_compile_tac_binary_op_neq_false_8bit();
	test_compile_tac_binary_op_neq_false_16bit();

	test_compile_tac_binary_op_lt_true_8bit();
	test_compile_tac_binary_op_lt_true_16bit();

	test_compile_tac_binary_op_lt_false_8bit();
	test_compile_tac_binary_op_lt_false_16bit();

	test_compile_tac_binary_op_eq_true_8bit();
	test_compile_tac_binary_op_eq_true_16bit();

	test_compile_tac_binary_op_eq_false_8bit();
	test_compile_tac_binary_op_eq_false_16bit();
}

static void test_compile_tac_binary_op_add_8bit(){

	status_test_codegen("TAC_BINARY_OP + (8 bit)");

	const int8_t start = rand()%0xff;

	for(int8_t change = 0; change < 100; change += 10){

		const int8_t expected = start + change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACConst(1, change));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_ADD, 1));

		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system,8);

		const int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

static void test_compile_tac_binary_op_add_16bit(){

	status_test_codegen("TAC_BINARY_OP + (16 bit)");

	const int16_t start = rand()%0xffff;

	for(int16_t change = 0; change < 1000; change += 100){

		const int16_t expected = start + change;

		//printf("%d + %d == %d\n", start, change, expected);

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACConst(1, change));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_ADD, 1));

		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system,8);

		const int8_t r0 = vmcu_system_read_gpr(system, 0);
		const int8_t r1 = vmcu_system_read_gpr(system, 1);

		assert(make16(r1,r0) == expected);

		vmcu_system_dtor(system);
	}
}

static void test_compile_tac_binary_op_sub_8bit(){

	status_test_codegen("TAC_BINARY_OP - (8 bit)");

	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start - change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_SUB, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == expected);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_sub_16bit(){

	status_test_codegen("TAC_BINARY_OP - (16 bit)");

	int16_t start = rand()%0xffff;
	int16_t change = rand()%0xffff;
	int16_t expected = start - change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_SUB, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	const uint8_t r0 = vmcu_system_read_gpr(system, 0);
	const uint8_t r1 = vmcu_system_read_gpr(system, 1);

	assert(make16(r1,r0) == expected);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_and_8bit(){

	status_test_codegen("TAC_BINARY_OP & (8 bit)");

	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start & change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_AND, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == expected);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_and_16bit(){

	status_test_codegen("TAC_BINARY_OP & (16 bit)");

	int16_t start = rand()%0xff;
	int16_t change = rand()%0xff;
	int16_t expected = start & change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_AND, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);
	int8_t r1 = vmcu_system_read_gpr(system, 1);

	assert(make16(r1,r0) == expected);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_or_8bit(){

	status_test_codegen("TAC_BINARY_OP | (8 bit)");

	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start | change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_OR, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == expected);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_or_16bit(){

	status_test_codegen("TAC_BINARY_OP | (16 bit)");

	int16_t start = rand()%0xffff;
	int16_t change = rand()%0xffff;
	int16_t expected = start | change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_OR, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	uint8_t r0 = vmcu_system_read_gpr(system, 0);
	uint8_t r1 = vmcu_system_read_gpr(system, 1);

	assert(make16(r1,r0) == expected);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_xor_8bit(){

	status_test_codegen("TAC_BINARY_OP ^ (8 bit)");

	int8_t start = rand()%0xff;
	int8_t change = rand()%0xff;
	int8_t expected = start ^ change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_XOR, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == expected);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_xor_16bit(){

	status_test_codegen("TAC_BINARY_OP ^ (16 bit)");

	uint16_t start = rand()%0xffff;
	uint16_t change = rand()%0xffff;
	uint16_t expected = start ^ change;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACConst(1, change));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_XOR, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	uint8_t r0 = vmcu_system_read_gpr(system, 0);
	uint8_t r1 = vmcu_system_read_gpr(system, 1);

	uint16_t actual = make16(r1, r0);

	//printf("0x%x ^ 0x%x == 0x%x (actual: 0x%x)\n", start, change, expected, actual);

	assert(actual == expected);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_neq_true_8bit(){

	status_test_codegen("TAC_BINARY_OP != true (8 bit)");

	int8_t value1 = rand()%0xf;
	int8_t value2 = value1+1;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_NEQ, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 != 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_neq_true_16bit(){

	status_test_codegen("TAC_BINARY_OP != true (16 bit)");

	int16_t value1 = 0x1 << 8;
	int16_t value2 = 0x1 << 9;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_NEQ, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 != 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_neq_false_8bit(){

	status_test_codegen("TAC_BINARY_OP != false (8 bit)");

	int8_t value1 = rand()%0xf;
	int8_t value2 = value1;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_NEQ, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_neq_false_16bit(){

	status_test_codegen("TAC_BINARY_OP != false (16 bit)");

	int16_t value1 = rand()%0xffff;
	int16_t value2 = value1;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_NEQ, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 8);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_lt_true_8bit(){

	status_test_codegen("TAC_BINARY_OP < true (8 bit)");

	int8_t value1 = rand()%0xf;
	int8_t value2 = value1+1;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_LT, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 != 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_lt_true_16bit(){

	status_test_codegen("TAC_BINARY_OP < true (16 bit)");

	int16_t value1 = 0x1 << 8;
	int16_t value2 = 0x1 << 9;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_LT, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 != 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_lt_false_8bit(){

	status_test_codegen("TAC_BINARY_OP < false (8 bit)");

	int8_t value1 = rand()%0xf;
	int8_t value2 = value1;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_LT, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_lt_false_16bit(){

	status_test_codegen("TAC_BINARY_OP < false (16 bit)");

	int16_t value1 = 0x1 << 8;
	int16_t value2 = 0x1 << 8;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_LT, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_eq_true_8bit(){

	status_test_codegen("TAC_BINARY_OP == true (8 bit)");

	int8_t value1 = rand()%0xff;
	int8_t value2 = value1;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_EQ, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 != 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_eq_true_16bit(){

	status_test_codegen("TAC_BINARY_OP == true (16 bit)");

	int16_t value1 = 0xff00 | rand()%0xff;
	int16_t value2 = value1;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_EQ, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 != 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_eq_false_8bit(){

	status_test_codegen("TAC_BINARY_OP == false (8 bit)");

	int8_t value1 = rand()%0xff;
	int8_t value2 = value1+1;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_EQ, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 0);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_eq_false_16bit(){

	status_test_codegen("TAC_BINARY_OP == false (16 bit)");

	int16_t value1 = 0x0100;
	int16_t value2 = 0x0200;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value1));
	tacbuffer_append(b, makeTACConst(1, value2));
	tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_EQ, 1));

	tacbuffer_append(b, makeTACReturn(0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 0);

	vmcu_system_dtor(system);
}
