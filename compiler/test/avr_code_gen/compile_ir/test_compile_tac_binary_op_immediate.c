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

void test_compile_tac_binary_op_immediate_case_add_8bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE + (8 bit)");

	int8_t start = 0x83;

	for (uint8_t change = 0; change < 5; change++) {

		int8_t expected = start + change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_ADD, change));

		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_add_16bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE + (16 bit)");

	int16_t start = 309;

	//printf("start = %d (0x%x)\n", start, start);

	for (uint16_t change = 0; change < 5; change++) {

		//printf("change = %d (0x%x)\n", change, change);

		int16_t expected = start + change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_ADD, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 20);

		int16_t actual = vmcu_system_read_2_gpr(system, 0);

		//printf("actual %d (0x%x)\n", actual, actual);
		//printf("expected %d (0x%x)\n", expected, expected);

		assert(expected == actual);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_sub_8bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE - (8 bit)");

	int8_t start = 44;

	for (uint8_t change = 0; change < 5; change++) {

		const int8_t expected = start - change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SUB, change));

		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_sub_16bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE - (16 bit)");

	int16_t start = 3483;

	for (uint8_t change = 0; change < 5; change++) {

		const int16_t expected = start - change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SUB, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int16_t actual = vmcu_system_read_2_gpr(system, 0);

		//printf("expected: %d\n", expected);
		//printf("actual: %d\n", actual);

		assert(expected == actual);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_and_8bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE & (8 bit)");

	int8_t start = 0xe3;
	for (int8_t change = 0; change < 5; change++) {
		int8_t expected = start & change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_AND, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_and_16bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE & (16 bit)");

	uint16_t start = 0xabcd;
	for (uint16_t change = 0x1001; change < 0x1006; change++) {
		uint16_t expected = start & change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_AND, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 12);

		int16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_or_8bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE | (8 bit)");

	int8_t start = 0xc7;
	for (int8_t change = 0; change < 5; change++) {
		int8_t expected = start | change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_OR, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_or_16bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE | (16 bit)");

	uint16_t start = 0xabcd;
	for (uint16_t change = 0x1000; change < 0x1005; change++) {
		uint16_t expected = start | change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_OR, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		uint16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_xor_8bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE ^ (8 bit)");

	int8_t start = 0xc3;
	for (int8_t change = 0; change < 5; change++) {
		int8_t expected = start ^ change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_XOR, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		int8_t r0 = vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_xor_16bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE ^ (16 bit)");

	uint16_t start = 0xabcd;
	for (uint16_t change = 0x1000; change < 0x1005; change++) {
		uint16_t expected = start ^ change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_XOR, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		uint16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_shift_left_8bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE << (8 bit)");

	uint8_t start = 0xb3;

	for (int change = 1; change < 6; change++) {

		const uint8_t expected = start << change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SHIFT_LEFT, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8 + change);

		uint8_t r0 = (uint8_t)vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_shift_left_16bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE << (16 bit)");

	uint16_t start = 0xabcd;

	for (int change = 1; change < 6; change++) {

		const uint16_t expected = start << change;

		//printf("expected 0x%x\n", expected);

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SHIFT_LEFT, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8 + change * 2);

		uint16_t actual = vmcu_system_read_2_gpr(system, 0);

		//printf("actual 0x%x\n", actual);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_shift_right_8bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE >> (8 bit)");

	uint8_t start = 0xb4;

	for (int change = 1; change < 6; change++) {

		const uint8_t expected = start >> change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SHIFT_RIGHT, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8 + change);

		uint8_t r0 = (uint8_t)vmcu_system_read_gpr(system, 0);

		assert(r0 == expected);

		vmcu_system_dtor(system);
	}
}

void test_compile_tac_binary_op_immediate_case_shift_right_16bit() {

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE >> (16 bit)");

	uint16_t start = 0xabcd;

	for (int change = 1; change < 6; change++) {

		const uint16_t expected = start >> change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SHIFT_RIGHT, change));
		tacbuffer_append(b, makeTACReturn(0));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 8 + change * 2);

		uint16_t actual = vmcu_system_read_2_gpr(system, 0);

		assert(actual == expected);

		vmcu_system_dtor(system);
	}
}
