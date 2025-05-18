#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

void test_x86_compile_tac_if_goto_case_true() {

	status_test_x86_codegen("TAC_IF_GOTO true");

	const uint64_t value = 0x33 | 0x1;
	const uint64_t address1 = sd_uc_default_stack_addr() + 0x80;

	//labels
	const uint16_t l1 = 1;
	const uint16_t lend = 2;

	struct TACBuffer* b = tacbuffer_ctor();
	tacbuffer_append(b, makeTACSetupStackframe(0, 0));

	tacbuffer_append(b, makeTACConst(0, 1, 1));
	tacbuffer_append(b, makeTACIfGoto(0, 1, l1));
	tacbuffer_append(b, makeTACGoto(0, lend));

	tacbuffer_append(b, makeTACLabel(0, l1));
	tacbuffer_append(b, makeTACConst(0, 0, value));

	const uint8_t width = 1;
	tacbuffer_append(b, makeTACConst(0, 2, address1));
	tacbuffer_append(b, makeTACStore(0, 2, 0, width));
	tacbuffer_append(b, makeTACGoto(0, lend));

	tacbuffer_append(b, makeTACLabel(0, lend));
	tacbuffer_append(b, makeTACReturn(0, 0));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

	sd_uc_emu_start(system, 0, false);

	uint64_t read = 0;
	sd_uc_mem_read64(system, address1, &read);
	assert(read == value);

	sd_uc_close(system);
}

void test_x86_compile_tac_if_goto_case_false() {

	status_test_x86_codegen("TAC_IF_GOTO false");

	const int8_t value = 0x22 | 0x1;
	const uint64_t address1 = sd_uc_default_stack_addr() + 0x80;

	//labels
	const uint16_t l1 = 1;
	const uint16_t lend = 2;

	struct TACBuffer* b = tacbuffer_ctor();
	tacbuffer_append(b, makeTACSetupStackframe(0, 0));

	tacbuffer_append(b, makeTACConst(0, 1, 0));
	tacbuffer_append(b, makeTACIfGoto(0, 1, l1));
	tacbuffer_append(b, makeTACGoto(0, lend));

	tacbuffer_append(b, makeTACLabel(0, l1));
	tacbuffer_append(b, makeTACConst(0, 0, value));

	const uint8_t width = 2;
	tacbuffer_append(b, makeTACConst(0, 2, address1));
	tacbuffer_append(b, makeTACStore(0, 2, 0, width));

	tacbuffer_append(b, makeTACLabel(0, lend));
	tacbuffer_append(b, makeTACReturn(0, 0));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

	sd_uc_emu_start(system, 0, false);

	uint64_t read = 0;
	sd_uc_mem_read64(system, address1, &read);
	assert((int)read != (int)value);

	sd_uc_close(system);
}

void test_x86_compile_tac_if_goto_case_mixed() {

	status_test_x86_codegen("TAC_IF_GOTO mixed");

	//we need to test 2 cases
	// - condition true, we branch
	// - condition false, we do not branch
	//we check by writing a fixed value to 2 known addresses in data space

	const uint8_t value = 0xab | 0x1;
	const uint64_t address1 = sd_uc_default_stack_addr() + 0x80;
	const uint64_t address2 = address1 + 8;

	//labels
	const uint16_t l1 = 1;
	const uint16_t lend = 2;

	struct TACBuffer* b = tacbuffer_ctor();
	tacbuffer_append(b, makeTACSetupStackframe(0, 0));

	tacbuffer_append(b, makeTACConst(0, 0, value));

	tacbuffer_append(b, makeTACConst(0, 1, 1));
	tacbuffer_append(b, makeTACIfGoto(0, 1, l1)); //should branch
	tacbuffer_append(b, makeTACGoto(0, lend));

	tacbuffer_append(b, makeTACLabel(0, l1));
	tacbuffer_append(b, makeTACConst(0, 0, value));

	const uint8_t width = 2;
	tacbuffer_append(b, makeTACConst(0, 3, address1));
	tacbuffer_append(b, makeTACStore(0, 3, 0, width));

	tacbuffer_append(b, makeTACConst(0, 2, 0));
	tacbuffer_append(b, makeTACIfGoto(0, 2, lend)); //should not branch
	tacbuffer_append(b, makeTACConst(0, 0, value));

	tacbuffer_append(b, makeTACConst(0, 4, address2));
	tacbuffer_append(b, makeTACStore(0, 4, 0, width));
	tacbuffer_append(b, makeTACGoto(0, lend));

	tacbuffer_append(b, makeTACLabel(0, lend));
	tacbuffer_append(b, makeTACReturn(0, 0));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);

	sd_uc_emu_start(system, 0, false);

	//check that the values have arrived
	uint64_t read = 0;
	sd_uc_mem_read64(system, address1, &read);
	assert(read == value);
	sd_uc_mem_read64(system, address2, &read);
	assert(read == value);

	sd_uc_close(system);
}
