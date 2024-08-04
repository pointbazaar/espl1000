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

void test_x86_compile_tac_load() {

	status_test_x86_codegen("TAC_LOAD");

	test_fixed_value(0x00330033);
	test_fixed_value(0x00220022);
	test_fixed_value(0x00120012);
}

static void test_fixed_value(uint64_t fixed_value) {

	const uint64_t addr = sd_uc_default_start_addr() + 0x80;
	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, 0x00));
	tacbuffer_append(b, makeTACConst(2, addr));
	tacbuffer_append(b, makeTACLoad(1, 2));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer(b);

	uc_err err;
	err = sd_uc_mem_write64(system, addr, &fixed_value);
	assert(err == UC_ERR_OK);

	err = sd_uc_emu_start(system, 0, false);

	//check that the value was written to any register
	bool found = false;

	int regs[] = {
	    UC_X86_REG_RAX,
	    UC_X86_REG_RBX,
	    UC_X86_REG_RCX,
	    UC_X86_REG_RDX,
	};

	uint64_t reg = 0;
	for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
		err = sd_uc_reg_read(system, regs[i], &reg);
		assert(err == UC_ERR_OK);
		if (reg == fixed_value) found = true;
	}

	assert(found);

	sd_uc_close(system);
}
