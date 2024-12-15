#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

static void test_param(uint64_t fixed_value, bool debug);

void test_x86_compile_tac_param() {

	status_test_x86_codegen("TAC_PARAM");

	test_param(0x003c, false);
	test_param(0x5a00, false);
	test_param(0x120000, false);
}

static void test_param(uint64_t fixed_value, bool debug) {

	//test that the value gets pushed
	//and the stack pointer decrements

	struct TACBuffer* b = tacbuffer_ctor();

	const size_t stackframe_size = 4;
	const size_t stackframe_size_bytes = stackframe_size * 8;

	tacbuffer_append(b, makeTACSetupStackframe(stackframe_size_bytes));
	tacbuffer_append(b, makeTACConst(0, fixed_value));
	tacbuffer_append(b, makeTACConst(1, 0));
	tacbuffer_append(b, makeTACParam(0, false));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v3(b, debug, true, stackframe_size);

	uc_err err;

	if (debug) {
		sd_uc_print_regs(system);
		sd_uc_print_stack(system);
	}

	err = sd_uc_emu_start(system, 0, debug);

	uint64_t rsp;

	err = sd_uc_reg_read(system, UC_X86_REG_RSP, &rsp);
	assert(err == UC_ERR_OK);

	uint64_t read_value;
	err = sd_uc_mem_read64(system, rsp, &read_value);

	if (debug) {
		sd_uc_print_regs(system);
		sd_uc_print_stack(system);
	}

	//assert that fixed_value is on the stack
	assert(read_value == fixed_value);

	sd_uc_close(system);
}
