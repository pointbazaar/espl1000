#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void test_fixed_value(uint64_t fixed_value);

void test_x86_compile_tac_load_const_addr() {

	status_test_x86_codegen("TAC_LOAD_CONST_ADDR");

	test_fixed_value(0x55);
	test_fixed_value(0x5544);
	test_fixed_value(0x554433);
	test_fixed_value(0x55443322);
}

static void test_fixed_value(uint64_t fixed_value) {

	const uint64_t addr = sd_uc_default_start_addr() + 0x80;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, 0x00));
	tacbuffer_append(b, makeTACLoadConstAddr(1, addr));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer(b);

	uc_err err;
	//write value to be read later
	err = sd_uc_mem_write64(system, addr, &fixed_value);
	assert(err == UC_ERR_OK);

	err = sd_uc_emu_start(system, 0, false);
	assert(err == UC_ERR_OK);

	assert(sd_uc_some_reg_has_value(system, fixed_value));

	sd_uc_close(system);
}
