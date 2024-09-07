#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <unicorn/unicorn.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void test_x86_compile_tac_const_value_fixed_value(int8_t fixed_value);

void test_x86_compile_tac_const_value() {

	status_test_x86_codegen("TAC_CONST_VALUE");

	for (int8_t fixed_value = -2; fixed_value <= 2; fixed_value++) {

		test_x86_compile_tac_const_value_fixed_value(fixed_value * 10);
	}
}

static void test_x86_compile_tac_const_value_fixed_value(int8_t fixed_value) {

	struct TACBuffer* b = tacbuffer_ctor();
	assert(b != NULL);

	tacbuffer_append(b, makeTACNop());
	tacbuffer_append(b, makeTACConst(1, fixed_value));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, false);
	assert(system != NULL);

	uc_err err;

	err = sd_uc_emu_start(system, 0, false);
	if (err != UC_ERR_OK) {
		printf("error: %s\n", uc_strerror(err));
		assert(false);
	}

	assert(sd_uc_some_reg_has_value(system, fixed_value));

	sd_uc_close(system);
}
