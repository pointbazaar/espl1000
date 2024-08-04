#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void test_fixed_value(uint64_t fixed_value);

void test_x86_compile_tac_copy() {

	status_test_x86_codegen("TAC_COPY");

	test_fixed_value(0x03);
	test_fixed_value(0x04);
	test_fixed_value(10);
	test_fixed_value(19);
}

static void test_fixed_value(uint64_t fixed_value) {

	struct TACBuffer* b = tacbuffer_ctor();
	const uint64_t addr = sd_uc_default_start_addr() + 1000;

	tacbuffer_append(b, makeTACConst(0, fixed_value));
	tacbuffer_append(b, makeTACCopy(1, 0));

	tacbuffer_append(b, makeTACStoreConstAddr(addr, 1));

	struct sd_uc_engine* sduc = sd_uc_engine_from_tacbuffer(b);

	uc_err err;
	// prep the address
	uint64_t prep = 0;
	err = sd_uc_mem_write64(sduc, addr, &prep);
	if (err != UC_ERR_OK) {
		printf("error: %s\n", uc_strerror(err));
		assert(false);
	}

	err = sd_uc_emu_start(sduc, 0, false);

	if (err != UC_ERR_OK) {
		printf("error: %s\n", uc_strerror(err));
		assert(false);
	}

	uint64_t value;
	err = sd_uc_mem_read64(sduc, addr, &value);

	if (err != UC_ERR_OK) {
		printf("error: %s\n", uc_strerror(err));
		assert(false);
	}

	if (value != fixed_value) {
		printf("fixed_value=%04lx\n", fixed_value);
		sd_uc_print_regs(sduc);
	}

	assert(value == fixed_value);

	sd_uc_close(sduc);
}
