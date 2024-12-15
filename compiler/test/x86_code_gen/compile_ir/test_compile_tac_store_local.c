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

void test_x86_compile_tac_store_local() {

	status_test_x86_codegen("TAC_STORE_LOCAL");

	test_fixed_value(0x00330033, 0, false);
	test_fixed_value(0x00220022, 0, false);
	test_fixed_value(0x00470088, 1, false);
	test_fixed_value(0x00120012, 3, false);
}

static void test_fixed_value(uint64_t fixed_value, const uint32_t local_index, bool debug) {

	uc_err err;
	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACSetupStackframe(TEST_FAKE_STACKFRAME_SIZE_BYTES));
	tacbuffer_append(b, makeTACConst(1, fixed_value));
	tacbuffer_append(b, makeTACStoreLocal(local_index, 1));
	tacbuffer_append(b, makeTACReturn(1));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v3(b, debug, true);
	assert(system != NULL);

	if (debug) {
		sd_uc_print_regs(system);
		sd_uc_print_stack(system);
	}

	err = sd_uc_emu_start(system, 0, debug);
	assert(err == UC_ERR_OK);

	const uint64_t addr = sd_uc_default_stack_addr() - 8 * (local_index + 1);

	uint64_t value_read;
	err = sd_uc_mem_read64(system, addr, &value_read);
	assert(err == UC_ERR_OK);

	if (debug) {
		sd_uc_print_regs(system);
		sd_uc_print_stack(system);

		printf("[debug] read [0x%lx] == 0x%lx\n", addr, value_read);
	}

	sd_uc_close(system);

	assert(value_read == fixed_value);
}
