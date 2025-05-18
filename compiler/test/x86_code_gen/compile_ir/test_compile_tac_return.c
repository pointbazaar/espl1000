#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

static void test_fixed_value(uint32_t value, bool debug);

void test_x86_compile_tac_return() {

	status_test_x86_codegen("TAC_RETURN");

	test_fixed_value(0x33054, false);
	test_fixed_value(0x22055, false);
	test_fixed_value(0x11065, false);
}

static void test_fixed_value(uint32_t value, bool debug) {

	uc_err err;
	struct TACBuffer* b = tacbuffer_ctor();

	const size_t stackframe_size = 4;
	const size_t stackframe_size_bytes = stackframe_size * 8;

	tacbuffer_append(b, makeTACSetupStackframe(0, stackframe_size_bytes));
	tacbuffer_append(b, makeTACConst(0, 0, value + 1));
	tacbuffer_append(b, makeTACConst(0, 1, value));
	tacbuffer_append(b, makeTACConst(0, 2, value - 1));

	tacbuffer_append(b, makeTACCopy(0, 2, 0));
	tacbuffer_append(b, makeTACCopy(0, 0, 2));
	tacbuffer_append(b, makeTACReturn(0, 1));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v3(b, debug, true, stackframe_size);

	err = sd_uc_emu_start(system, 0, debug);

	assert(err == UC_ERR_OK);

	uint64_t reg;
	err = sd_uc_reg_read(system, UC_X86_REG_RAX, &reg);

	assert(err == UC_ERR_OK);

	assert(reg == value);

	sd_uc_close(system);
}
