#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void test_at_addr(uint64_t addr);

void test_x86_compile_tac_store_const_addr() {

	status_test_x86_codegen("TAC_STORE_CONST_ADDR");

	const uint64_t start = sd_uc_default_start_addr() + 0x80;

	test_at_addr(start);
	test_at_addr(start + 1);
	test_at_addr(start + 2);
	test_at_addr(start + 3);
}

static void test_at_addr(uint64_t addr) {

	const uint64_t fixed_value = 0x44abcdef;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, fixed_value));
	tacbuffer_append(b, makeTACStoreConstAddr(addr, 1));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer(b);
	assert(system != NULL);

	uc_err err;
	err = sd_uc_emu_start(system, 0, false);
	assert(err == UC_ERR_OK);

	//check that the value was written
	uint64_t value = 0;
	err = uc_mem_read(system->uc, addr, &value, sizeof(uint64_t));
	assert(err == UC_ERR_OK);

	assert(value == fixed_value);

	uc_close(system->uc);
}
