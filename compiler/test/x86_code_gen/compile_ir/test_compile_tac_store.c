#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void common(uint64_t addr, uint64_t fixed_value);

void test_x86_compile_tac_store() {

	status_test_x86_codegen("TAC_STORE");

	const uint64_t start = sd_uc_default_start_addr() + 0x80;

	for (uint64_t addr = start; addr < start + 0x5; addr++) {

		common(addr, 1000);
		common(addr, 1001);
		common(addr, 1002);
		common(addr, 1003);
	}
}

static void common(uint64_t addr, uint64_t fixed_value) {

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, fixed_value));
	tacbuffer_append(b, makeTACConst(2, addr));
	tacbuffer_append(b, makeTACStore(2, 1));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer(b);

	uc_err err;
	err = sd_uc_emu_start(system, 0, false);
	assert(err == UC_ERR_OK);

	uint64_t stored = 0;
	err = uc_mem_read(system->uc, addr, &stored, sizeof(uint64_t));
	assert(err == UC_ERR_OK);

	assert(stored == fixed_value);

	uc_close(system->uc);
}