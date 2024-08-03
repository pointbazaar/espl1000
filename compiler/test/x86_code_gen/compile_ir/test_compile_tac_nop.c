#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

void test_x86_compile_tac_nop() {

	status_test_x86_codegen("TAC_NOP");

	//test that register etc. do not change with TAC_NOP
	//this test is also intended for completeness

	struct TACBuffer* b = tacbuffer_ctor();
	int n = 8;

	for (int i = 0; i < n; i++) {
		tacbuffer_append(b, makeTACNop());
	}

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer(b);

	//record register values
	int8_t regs[32];
	for (int i = UC_X86_REG_RAX; i <= UC_X86_REG_RDX; i++) {
		int64_t reg;
		uc_reg_read(system->uc, i, &reg);
		regs[i] = reg;
	}

	uint64_t old_sp;
	uc_reg_read(system->uc, UC_X86_REG_RSP, &old_sp);

	//do some steps
	sd_uc_emu_start(system, 0, false);

	//check that the values are still the same
	for (int i = UC_X86_REG_RAX; i <= UC_X86_REG_RDX; i++) {
		int64_t reg;
		uc_reg_read(system->uc, i, &reg);
		assert(regs[i] == reg);
	}

	uint64_t sp;
	uc_reg_read(system->uc, UC_X86_REG_RSP, &sp);
	assert(old_sp == sp);

	uc_close(system->uc);
}
