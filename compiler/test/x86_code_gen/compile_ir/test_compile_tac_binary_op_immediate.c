#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

void test_x86_compile_tac_binary_op_immediate_case_add() {

	status_test_x86_codegen("TAC_BINARY_OP_IMMEDIATE +");

	int8_t start = 0x83;

	for (uint8_t change = 0; change < 5; change++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_ADD, change));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == (start + change));

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_immediate_case_sub() {

	status_test_x86_codegen("TAC_BINARY_OP_IMMEDIATE -");

	int8_t start = 44;

	for (uint8_t change = 0; change < 5; change++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SUB, change));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == (start - change));

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_immediate_case_and() {

	status_test_x86_codegen("TAC_BINARY_OP_IMMEDIATE &");

	int8_t start = 0xe3;
	for (int8_t change = 0; change < 5; change++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_AND, change));
		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == (start & change));

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_immediate_case_or() {

	status_test_x86_codegen("TAC_BINARY_OP_IMMEDIATE |");

	int8_t start = 0xc7;
	for (int8_t change = 0; change < 5; change++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_OR, change));
		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == (start | change));

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_immediate_case_xor() {

	status_test_x86_codegen("TAC_BINARY_OP_IMMEDIATE ^");

	int8_t start = 0xc3;
	for (int8_t change = 0; change < 5; change++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_XOR, change));
		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == (start ^ change));

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_immediate_case_shift_left() {

	status_test_x86_codegen("TAC_BINARY_OP_IMMEDIATE <<");

	uint64_t start = 0xb3;

	for (int change = 1; change < 6; change++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SHIFT_LEFT, change));
		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == (start << change));

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_immediate_case_shift_right() {

	status_test_x86_codegen("TAC_BINARY_OP_IMMEDIATE >>");

	uint64_t start = 0xb4;

	for (int change = 1; change < 6; change++) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACBinOpImmediate(0, TAC_OP_SHIFT_RIGHT, change));
		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == (start >> change));

		sd_uc_close(system);
	}
}
