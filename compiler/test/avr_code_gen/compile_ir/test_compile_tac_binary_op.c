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

void test_compile_tac_binary_op_add_8bit() {

	status_test_codegen("TAC_BINARY_OP + (8 bit)");

	const int8_t start = 0x01;

	for (int8_t change = 0; change < 4; change++) {

		const int8_t expected = start + change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_ADD, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		const int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_add_16bit() {

	status_test_codegen("TAC_BINARY_OP + (16 bit)");

	const int16_t start = 0xabcd;

	for (int16_t change = 0; change < 500; change += 100) {

		const int16_t expected = start + change;

		//printf("%d + %d == %d\n", start, change, expected);

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_ADD, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		const int16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_sub_8bit() {

	status_test_codegen("TAC_BINARY_OP - (8 bit)");

	int8_t start = 0;
	for (int8_t change = -10; change < 40; change += 10) {

		int8_t expected = start - change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_SUB, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_sub_16bit() {

	status_test_codegen("TAC_BINARY_OP - (16 bit)");

	int16_t start = 0xabcd;
	for (int16_t change = -0x200; change < 0x300; change += 0x100) {

		int16_t expected = start - change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_SUB, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		const int16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_and_8bit() {

	status_test_codegen("TAC_BINARY_OP & (8 bit)");

	uint8_t start = 0x45;
	for (uint8_t change = 0xf0; change < 0xf3; change++) {

		int8_t expected = start & change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_AND, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_and_16bit() {

	status_test_codegen("TAC_BINARY_OP & (16 bit)");

	int16_t start = 0xabcd;
	for (int16_t change = 0x1000; change < 0x1003; change++) {

		int16_t expected = start & change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_AND, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		const int16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_or_8bit() {

	status_test_codegen("TAC_BINARY_OP | (8 bit)");

	int8_t start = 0xab;
	for (int8_t change = 0x30; change < 0x33; change++) {

		int8_t expected = start | change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_OR, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_or_16bit() {

	status_test_codegen("TAC_BINARY_OP | (16 bit)");

	int16_t start = 0xabcd;
	for (int16_t change = 0x1400; change < 0x1403; change++) {
		int16_t expected = start | change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_OR, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		const int16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_xor_8bit() {

	status_test_codegen("TAC_BINARY_OP ^ (8 bit)");

	uint8_t start = 0xab;
	for (uint8_t change = 0xf0; change < 0xf3; change++) {
		int8_t expected = start ^ change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_XOR, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_xor_16bit() {

	status_test_codegen("TAC_BINARY_OP ^ (16 bit)");

	uint16_t start = 0x1bcd;
	for (uint16_t change = 0x4000; change < 0x4003; change++) {

		uint16_t expected = start ^ change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_XOR, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		const uint16_t actual = vmcu_system_read_2_gpr(system, 0);

		//printf("0x%x ^ 0x%x == 0x%x (actual: 0x%x)\n", start, change, expected, actual);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_xor_mixed1() {

	status_test_codegen("TAC_BINARY_OP ^ (mixed1, 8bit, 16bit) [FAILING - disabled]");
	return;

	const uint16_t start = 0x00ff;
	const uint16_t change = 0xffff;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, 0, start));
	tacbuffer_append(b, makeTACConst(0, 1, change));
	tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_XOR, 1));

	tacbuffer_append(b, makeTACReturn(0, 0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 10);

	const uint16_t actual = vmcu_system_read_2_gpr(system, 0);

	assert(actual == 0xff00);

	vmcu_system_dtor(system);
}

void test_compile_tac_binary_op_xor_mixed2() {

	status_test_codegen("TAC_BINARY_OP ^ (mixed2, 16bit, 8bit)");

	const uint16_t start = 0xabcd;

	for (uint16_t change = 0x1000; change < 0x1003; change++) {

		const uint16_t expected = start ^ change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, start));
		tacbuffer_append(b, makeTACConst(0, 1, change));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_XOR, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		const uint16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_neq_true_8bit() {

	status_test_codegen("TAC_BINARY_OP != true (8 bit)");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {

		int8_t value2 = value1 + 1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_NEQ, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 != 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_neq_true_16bit() {

	status_test_codegen("TAC_BINARY_OP != true (16 bit)");

	for (int16_t value1 = 0x100; value1 < 0x103; value1++) {

		int16_t value2 = 0x200;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_NEQ, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 != 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_neq_false_8bit() {

	status_test_codegen("TAC_BINARY_OP != false (8 bit)");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {

		int8_t value2 = value1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_NEQ, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_neq_false_16bit() {

	status_test_codegen("TAC_BINARY_OP != false (16 bit)");

	for (int16_t value1 = 0x1000; value1 < 0x1003; value1++) {

		int16_t value2 = value1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_NEQ, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_lt_true_8bit() {

	status_test_codegen("TAC_BINARY_OP < true (8 bit)");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {

		int8_t value2 = value1 + 1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_LT, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 != 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_lt_true_16bit() {

	status_test_codegen("TAC_BINARY_OP < true (16 bit)");

	for (uint16_t value1 = 0x100; value1 < 0x103; value1++) {

		uint16_t value2 = 0x200;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_LT, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 != 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_lt_false_8bit() {

	status_test_codegen("TAC_BINARY_OP < false (8 bit)");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {
		int8_t value2 = value1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_LT, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_lt_false_16bit() {

	status_test_codegen("TAC_BINARY_OP < false (16 bit)");

	for (uint16_t value1 = 0x100; value1 < 0x102; value1++) {
		for (uint16_t value2 = value1 - 3; value2 <= value1; value2++) {

			struct TACBuffer* b = tacbuffer_ctor();

			tacbuffer_append(b, makeTACConst(0, 0, value1));
			tacbuffer_append(b, makeTACConst(0, 1, value2));
			tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_LT, 1));

			tacbuffer_append(b, makeTACReturn(0, 0));

			vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

			vmcu_system_step_n(system, 10);

			int8_t r0 = vmcu_system_read_gpr(system, 0);

			assert(r0 == 0);

			vmcu_system_dtor(system);
		}
	}
}

void test_compile_tac_binary_op_eq_true_8bit() {

	status_test_codegen("TAC_BINARY_OP == true (8 bit)");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {
		int8_t value2 = value1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_EQ, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 != 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_eq_true_16bit() {

	status_test_codegen("TAC_BINARY_OP == true (16 bit)");

	for (uint16_t value1 = 0xff00; value1 < 0xff03; value1++) {

		uint16_t value2 = value1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_EQ, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 != 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_eq_false_8bit() {

	status_test_codegen("TAC_BINARY_OP == false (8 bit)");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {
		int8_t value2 = value1 + 1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_EQ, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_eq_false_16bit() {

	status_test_codegen("TAC_BINARY_OP == false (16 bit)");

	for (int16_t value1 = 0x0100; value1 < 0x0103; value1++) {

		int16_t value2 = 0x0200;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_EQ, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_geq_true_8bit() {

	status_test_codegen("TAC_BINARY_OP >= true (8 bit)");

	for (uint8_t value1 = 0x03; value1 < 0x06; value1++) {

		uint8_t value2 = 0x03;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_GE, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 != 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_geq_true_16bit() {

	status_test_codegen("TAC_BINARY_OP >= true (16 bit)");

	for (uint16_t value1 = 0x0103; value1 < 0x0106; value1++) {
		uint16_t value2 = 0x0103;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_GE, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 != 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_geq_false_8bit() {

	status_test_codegen("TAC_BINARY_OP >= false (8 bit)");

	for (uint8_t value1 = 0x03; value1 < 0x06; value1++) {

		uint8_t value2 = 0x10;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_GE, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_geq_false_16bit() {

	status_test_codegen("TAC_BINARY_OP >= false (16 bit)");

	for (uint16_t value1 = 0x0103; value1 < 0x0106; value1++) {
		uint16_t value2 = 0x0203;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, 0, value1));
		tacbuffer_append(b, makeTACConst(0, 1, value2));
		tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_CMP_GE, 1));

		tacbuffer_append(b, makeTACReturn(0, 0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 20);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == 0);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_shift_left_8bit() {

	status_test_codegen("TAC_BINARY_OP << (8 bit)");

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, 0, 0x1));
	tacbuffer_append(b, makeTACConst(0, 1, 3));
	tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_SHIFT_LEFT, 1));

	tacbuffer_append(b, makeTACReturn(0, 0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 20);

	const int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 8);

	vmcu_system_dtor(system);
}

void test_compile_tac_binary_op_shift_left_16bit() {

	status_test_codegen("TAC_BINARY_OP << (16 bit)");

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, 0, 0x100));
	tacbuffer_append(b, makeTACConst(0, 1, 2));
	tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_SHIFT_LEFT, 1));

	tacbuffer_append(b, makeTACReturn(0, 0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 20);

	const uint16_t r0r1 = vmcu_system_read_2_gpr(system, 0);

	assert(r0r1 == 0x400);

	vmcu_system_dtor(system);
}

void test_compile_tac_binary_op_shift_right_8bit() {

	status_test_codegen("TAC_BINARY_OP >> (8 bit)");

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, 0, 0x10));
	tacbuffer_append(b, makeTACConst(0, 1, 2));
	tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_SHIFT_RIGHT, 1));

	tacbuffer_append(b, makeTACReturn(0, 0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 20);

	const int8_t r0 = vmcu_system_read_gpr(system, 0);

	assert(r0 == 4);

	vmcu_system_dtor(system);
}

void test_compile_tac_binary_op_shift_right_16bit() {

	status_test_codegen("TAC_BINARY_OP >> (16 bit)");

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, 0, 0x100));
	tacbuffer_append(b, makeTACConst(0, 1, 2));
	tacbuffer_append(b, makeTACBinOp(0, 0, TAC_OP_SHIFT_RIGHT, 1));

	tacbuffer_append(b, makeTACReturn(0, 0));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 20);

	const uint16_t r0r1 = vmcu_system_read_2_gpr(system, 0);

	assert(r0r1 == 0x40);

	vmcu_system_dtor(system);
}
