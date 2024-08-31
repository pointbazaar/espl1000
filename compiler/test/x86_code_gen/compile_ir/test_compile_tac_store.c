#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void common(uint64_t addr, uint64_t fixed_value, bool debug);

void test_x86_compile_tac_store() {

	status_test_x86_codegen("TAC_STORE");

	const uint64_t start = sd_uc_default_start_addr() + 0x80;

	common(start, 1000, false);
	common(start + 1, 1000, false);

	common(start, 1001, false);
	common(start + 2, 1001, false);

	common(start, 1002, false);
	common(start + 5, 1002, false);

	common(start, 1003, false);
	common(start + 9, 1003, false);
}

static void common(uint64_t addr, uint64_t fixed_value, bool debug) {

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, fixed_value));
	tacbuffer_append(b, makeTACConst(2, addr));
	tacbuffer_append(b, makeTACStore(2, 1));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, debug);

	uc_err err;
	err = sd_uc_emu_start(system, 0, false);
	assert(err == UC_ERR_OK);

	uint64_t stored = 0;
	err = sd_uc_mem_read64(system, addr, &stored);
	assert(err == UC_ERR_OK);

	assert(stored == fixed_value);

	sd_uc_close(system);
}
