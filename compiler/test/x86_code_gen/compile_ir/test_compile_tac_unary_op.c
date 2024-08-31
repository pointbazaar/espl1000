#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void case_minus(bool debug);
static void case_not(bool debug);
static void case_bitwise_neg(bool debug);

void test_x86_compile_tac_unary_op() {

	case_minus(false);
	case_not(false);
	case_bitwise_neg(false);
}

static void case_minus(bool debug) {

	status_test_x86_codegen("TAC_UNARY_OP -");

	for (int8_t start = -40; start < 40; start += 10) {

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACUnaryOp(1, 0, TAC_OP_UNARY_MINUS));
		tacbuffer_append(b, makeTACReturn(1));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, debug);

		sd_uc_emu_start(system, 0, debug);

		uint64_t rax = 1;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		assert(rax == -start);

		sd_uc_close(system);
	}
}

static void case_not(bool debug) {

	status_test_x86_codegen("TAC_UNARY_OP !");

	const int8_t start = 0xff; //start is true value

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, start));
	tacbuffer_append(b, makeTACUnaryOp(1, 0, TAC_OP_UNARY_NOT));
	tacbuffer_append(b, makeTACReturn(1));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, debug);

	sd_uc_emu_start(system, 0, debug);

	uint64_t rax = 1;
	sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

	if (debug) {
		printf("rax = 0x%lx (%ld)\n", rax, rax);
	}

	assert(rax == 0);

	sd_uc_close(system);
}

static void case_bitwise_neg(bool debug) {

	status_test_x86_codegen("TAC_UNARY_OP ~");

	for (uint64_t start = 0; start < 10; start++) {
		const uint64_t expect = ~start;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(0, start));
		tacbuffer_append(b, makeTACUnaryOp(1, 0, TAC_OP_UNARY_BITWISE_NEG));
		tacbuffer_append(b, makeTACReturn(1));

		struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, debug);

		sd_uc_emu_start(system, 0, debug);

		uint64_t rax = 1;
		sd_uc_reg_read(system, UC_X86_REG_RAX, &rax);

		if (debug) {
			printf("rax = 0x%lx (%ld)\n", rax, rax);
		}

		assert(rax == expect);

		sd_uc_close(system);
	}
}
