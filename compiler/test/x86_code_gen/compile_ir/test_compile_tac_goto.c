#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

void test_x86_compile_tac_goto() {

	status_test_x86_codegen("TAC_GOTO");

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACLabel(43));
	tacbuffer_append(b, makeTACGoto(43));
	tacbuffer_append(b, makeTACReturn(0));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer(b);

	sd_uc_emu_start(system, 7, false);

	uint64_t old_rip;
	uc_reg_read(system->uc, UC_X86_REG_RIP, &old_rip);

	sd_uc_emu_start(system, 7, false);

	//assert that the PC has not changed
	uint64_t new_rip;
	uc_reg_read(system->uc, UC_X86_REG_RIP, &new_rip);
	assert(new_rip == old_rip);

	uc_close(system->uc);
}
