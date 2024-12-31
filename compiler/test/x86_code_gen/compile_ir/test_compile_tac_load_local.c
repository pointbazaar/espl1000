#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void test_fixed_value(uint64_t fixed_value, const uint32_t local_index, bool debug);

void test_x86_compile_tac_load_local() {

	status_test_x86_codegen("TAC_LOAD_LOCAL");

	test_fixed_value(0x00330033, 0, false);
	test_fixed_value(0x00220022, 0, false);
	test_fixed_value(0x00120012, 1, false);
	test_fixed_value(0x00837500, 2, false);
}

static void test_fixed_value(uint64_t fixed_value, const uint32_t local_index, bool debug) {

	const uint64_t addr = sd_uc_default_stack_addr() - 8 * (local_index + 1);
	struct TACBuffer* b = tacbuffer_ctor();

	const size_t stackframe_size = 4;
	const size_t stackframe_size_bytes = stackframe_size * 8;

	tacbuffer_append(b, makeTACSetupStackframe(stackframe_size_bytes));
	tacbuffer_append(b, makeTACConst(2, fixed_value));
	tacbuffer_append(b, makeTACStoreLocal(local_index, 2));

	tacbuffer_append(b, makeTACConst(1, 0x00));
	tacbuffer_append(b, makeTACLoadLocal(1, local_index));
	tacbuffer_append(b, makeTACReturn(1));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v3(b, debug, true, stackframe_size);
	assert(system != NULL);

	uc_err err;
	uint64_t reg;

	if (debug) {
		sd_uc_print_regs(system);
		sd_uc_print_stack(system);
	}

	err = sd_uc_emu_start(system, 0, debug);
	assert(err == UC_ERR_OK);

	err = sd_uc_reg_read(system, UC_X86_REG_RAX, &reg);
	assert(err == UC_ERR_OK);

	if (debug) {
		sd_uc_print_regs(system);
		sd_uc_print_stack(system);
	}

	assert(reg == fixed_value);

	sd_uc_close(system);
}
