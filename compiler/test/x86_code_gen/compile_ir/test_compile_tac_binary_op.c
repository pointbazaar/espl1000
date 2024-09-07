#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

void test_x86_compile_tac_binary_op_add_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP +");

	const int8_t start = 0x01;

	for (int8_t change = 0; change < 3; change++) {

		const int8_t expected = start + change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACConst(1, change));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_ADD, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == expected);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_sub_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP -");

	int8_t start = 0;
	for (int8_t change = -2; change < 2; change++) {

		int8_t expected = start - change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACConst(1, change));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_SUB, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == expected);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_and_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP &");

	uint8_t start = 0x45;
	for (uint8_t change = 0xf0; change < 0xf3; change++) {

		int8_t expected = start & change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACConst(1, change));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_AND, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == expected);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_or_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP |");

	int8_t start = 0xab;
	for (int8_t change = 0x30; change < 0x33; change++) {

		int8_t expected = start | change;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACConst(1, change));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_OR, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == expected);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_xor() {

	status_test_x86_codegen("TAC_BINARY_OP ^");

	uint64_t start = 0xab;
	for (uint64_t change = 0xf0; change < 0xf3; change++) {
		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACConst(1, change));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_XOR, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == (start ^ change));

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_neq_true_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP != true");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {

		int8_t value2 = value1 + 1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, value1));
		tacbuffer_append(b, makeTACConst(1, value2));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_NEQ, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax != 0);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_neq_false_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP != false");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {

		int8_t value2 = value1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, value1));
		tacbuffer_append(b, makeTACConst(1, value2));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_NEQ, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == 0);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_lt_true_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP < true");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {

		int8_t value2 = value1 + 1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, value1));
		tacbuffer_append(b, makeTACConst(1, value2));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_LT, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax != 0);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_lt_false_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP < false");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {
		int8_t value2 = value1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, value1));
		tacbuffer_append(b, makeTACConst(1, value2));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_LT, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == 0);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_eq_true_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP == true");

	for (int8_t value1 = 0x0; value1 < 0x3; value1++) {
		int8_t value2 = value1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, value1));
		tacbuffer_append(b, makeTACConst(1, value2));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_EQ, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);
		uc_err err;

		err = sd_uc_emu_start(system, 0, false);
		assert(err == UC_ERR_OK);

		uint64_t rax = 0;
		err = sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);
		assert(err == UC_ERR_OK);

		assert(rax != 0);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_eq_false_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP == false");

	for (uint64_t value1 = 0x0; value1 < 0x3; value1++) {
		uint64_t value2 = value1 + 1;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, value1));
		tacbuffer_append(b, makeTACConst(1, value2));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_EQ, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);
		uc_err err;

		err = sd_uc_emu_start(system, 0, false);
		assert(err == UC_ERR_OK);

		uint64_t rax = 1;
		err = sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);
		assert(err == UC_ERR_OK);

		assert(rax == 0);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_geq_true_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP >= true");

	for (uint8_t value1 = 0x03; value1 < 0x06; value1++) {

		uint8_t value2 = 0x03;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, value1));
		tacbuffer_append(b, makeTACConst(1, value2));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_GE, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 0;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax != 0);

		sd_uc_close(system);
	}
}

void test_x86_compile_tac_binary_op_geq_false_8bit() {

	status_test_x86_codegen("TAC_BINARY_OP >= false");

	for (uint8_t value1 = 0x03; value1 < 0x06; value1++) {

		uint8_t value2 = 0x10;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, value1));
		tacbuffer_append(b, makeTACConst(1, value2));
		tacbuffer_append(b, makeTACBinOp(0, TAC_OP_CMP_GE, 1));

		tacbuffer_append(b, makeTACReturn(0));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

		sd_uc_emu_start(system, 0, false);

		uint64_t rax = 1;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == 0);

		sd_uc_close(system);
	}
}
