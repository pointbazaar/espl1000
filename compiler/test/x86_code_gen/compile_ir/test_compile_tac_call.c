#include <assert.h>

#include "tac/tacbuffer.h"
#include "tac/tac.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

static void test_call_no_args(uint32_t value, bool debug);

void test_x86_compile_tac_call() {

	status_test_x86_codegen("TAC_CALL (no args)");

	test_call_no_args(0x33054, false);
}

static void test_call_no_args(uint32_t value, bool debug) {

	uc_err err;
	struct TACBuffer* b = tacbuffer_ctor();
	struct TACBuffer* b2 = tacbuffer_ctor();

	const size_t stackframe_size = 0;
	const size_t stackframe_size_bytes = stackframe_size * 8;
	tacbuffer_append(b, makeTACLabelFunction(0));
	tacbuffer_append(b, makeTACSetupStackframe(stackframe_size_bytes));

	tacbuffer_append(b, makeTACCall(0, 1));
	tacbuffer_append(b, makeTACReturn(0));

	tacbuffer_append(b2, makeTACLabelFunction(1));
	tacbuffer_append(b2, makeTACSetupStackframe(stackframe_size_bytes));
	tacbuffer_append(b2, makeTACConst(2, value));
	tacbuffer_append(b2, makeTACReturn(2));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v4(b, b2, debug, true, stackframe_size, 0);

	err = sd_uc_emu_start(system, 0, debug);

	assert(err == UC_ERR_OK);

	uint64_t reg;
	err = sd_uc_reg_read(system, UC_X86_REG_RAX, &reg);

	if (reg != value) {
		fprintf(stderr, "rax == 0x%lx\n", reg);
	}

	assert(reg == value);

	sd_uc_close(system);
}
