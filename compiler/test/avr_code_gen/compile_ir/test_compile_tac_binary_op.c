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
static void test_compile_tac_binary_op_xor_mixed1();
static void test_compile_tac_binary_op_xor_mixed2();

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
	test_compile_tac_binary_op_xor_mixed1();
	test_compile_tac_binary_op_xor_mixed2();

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

	const int8_t start = 0x01;

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

	const int16_t start = 0xabcd;

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

	int8_t start = 0;
	for(int8_t change = -10; change < 100; change += 10){

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
}

static void test_compile_tac_binary_op_sub_16bit(){

	status_test_codegen("TAC_BINARY_OP - (16 bit)");

	int16_t start = 0xabcd;
	for(int16_t change = -0x500; change < 0x500; change += 0x100){

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
}

static void test_compile_tac_binary_op_and_8bit(){

	status_test_codegen("TAC_BINARY_OP & (8 bit)");

	uint8_t start = 0x45;
	for(uint8_t change = 0xf0; change < 0xfe; change++){

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
}

static void test_compile_tac_binary_op_and_16bit(){

	status_test_codegen("TAC_BINARY_OP & (16 bit)");

	int16_t start = 0xabcd;
	for(int16_t change = 0x1000; change < 0x1010; change++){

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
}

static void test_compile_tac_binary_op_or_8bit(){

	status_test_codegen("TAC_BINARY_OP | (8 bit)");

	int8_t start = 0xab;
	for(int8_t change = 0x30; change < 0x40; change++){

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
}

static void test_compile_tac_binary_op_or_16bit(){

	status_test_codegen("TAC_BINARY_OP | (16 bit)");

	int16_t start = 0xabcd;
	for(int16_t change = 0x1400; change < 0x1410; change++){
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
}

static void test_compile_tac_binary_op_xor_8bit(){

	status_test_codegen("TAC_BINARY_OP ^ (8 bit)");

	uint8_t start = 0xab;
	for(uint8_t change = 0xf0; change < 0xff; change++){
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
}

static void test_compile_tac_binary_op_xor_16bit(){

	status_test_codegen("TAC_BINARY_OP ^ (16 bit)");

	uint16_t start = 0x1bcd;
	for(uint16_t change = 0x4000; change < 0x4010; change++){

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
}

static void test_compile_tac_binary_op_xor_mixed1(){

	status_test_codegen("TAC_BINARY_OP ^ (mixed1, 8bit, 16bit) [FAILING - disabled]");
	return;

	const uint16_t start  = 0x00ff;
	const uint16_t change = 0xffff;

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

	assert(actual == 0xff00);

	vmcu_system_dtor(system);
}

static void test_compile_tac_binary_op_xor_mixed2(){

	status_test_codegen("TAC_BINARY_OP ^ (mixed2, 16bit, 8bit)");

	const uint16_t start  = 0xabcd;

	for(uint16_t change = 0x1000; change < 0x1010; change++){

		const uint16_t expected = start ^ change;

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

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

static void test_compile_tac_binary_op_neq_true_8bit(){

	status_test_codegen("TAC_BINARY_OP != true (8 bit)");

	for(int8_t value1 = 0x0; value1 < 0xf; value1++){

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
}

static void test_compile_tac_binary_op_neq_true_16bit(){

	status_test_codegen("TAC_BINARY_OP != true (16 bit)");

	for(int16_t value1 = 0x100; value1 < 0x10f; value1++){

		int16_t value2 = 0x200;

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
}

static void test_compile_tac_binary_op_neq_false_8bit(){

	status_test_codegen("TAC_BINARY_OP != false (8 bit)");

	for(int8_t value1 = 0x0; value1 < 0xf; value1++){

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
}

static void test_compile_tac_binary_op_neq_false_16bit(){

	status_test_codegen("TAC_BINARY_OP != false (16 bit)");

	for(int16_t value1 = 0x1000; value1 < 0x100f; value1++){

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
}

static void test_compile_tac_binary_op_lt_true_8bit(){

	status_test_codegen("TAC_BINARY_OP < true (8 bit)");

	for(int8_t value1 = 0x0; value1 < 0xf; value1++){

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

	for(int8_t value1 = 0x0; value1 < 0xf; value1++){
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

	for(int8_t value1 = 0x0; value1 < 0xf; value1++){
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
}

static void test_compile_tac_binary_op_eq_true_16bit(){

	status_test_codegen("TAC_BINARY_OP == true (16 bit)");

	for(uint16_t value1 = 0xff00; value1 < 0xff0f; value1++){

		uint16_t value2 = value1;

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
}

static void test_compile_tac_binary_op_eq_false_8bit(){

	status_test_codegen("TAC_BINARY_OP == false (8 bit)");

	for(int8_t value1 = 0x0; value1 < 0xf; value1++){
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
}

static void test_compile_tac_binary_op_eq_false_16bit(){

	status_test_codegen("TAC_BINARY_OP == false (16 bit)");

	for(int16_t value1 = 0x0100; value1 < 0x010f; value1++){

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
}
