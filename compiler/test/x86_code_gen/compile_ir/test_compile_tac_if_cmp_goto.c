#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "x86_code_gen/cg_x86.h"
#include "x86_code_gen/cg_x86_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_x86_code_gen.h"
#include "../test_x86_code_gen_util.h"

#include "test_compile_tac.h"

static void common(int a, enum TAC_OP op, int b, bool expect_true, bool debug);

static void common(int a1, enum TAC_OP op, int a2, bool expect_true, bool debug) {

	const int8_t valuetrue = 17;
	const int8_t valuefalse = valuetrue + 1;
	const uint64_t address = sd_uc_default_stack_addr() + 0x80;

	const uint16_t ltrue = 1;
	const uint16_t lfalse = 2;
	const uint16_t lend = 3;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACSetupStackframe(0));

	tacbuffer_append(b, makeTACConst(1, a1));
	tacbuffer_append(b, makeTACConst(2, a2));
	tacbuffer_append(b, makeTACIfCMPGoto(1, op, 2, ltrue));
	tacbuffer_append(b, makeTACGoto(lfalse));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(ltrue));
	tacbuffer_append(b, makeTACConst(0, valuetrue));
	tacbuffer_append(b, makeTACStoreConstAddr(address, 0));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(lfalse));
	tacbuffer_append(b, makeTACConst(0, valuefalse));
	tacbuffer_append(b, makeTACStoreConstAddr(address + 1, 0));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACReturn(1));

	struct sd_uc_engine* system = sd_uc_engine_from_tacbuffer_v2(b, debug);

	uint64_t read;
	uint64_t read2;
	uint64_t write = 0x0;
	sd_uc_mem_write64(system, address, &write);
	sd_uc_mem_write64(system, address + 1, &write);

	sd_uc_emu_start(system, 0, false);

	sd_uc_mem_read64(system, address, &read);
	sd_uc_mem_read64(system, address + 1, &read2);

	if (expect_true) {
		//true branch should have written valuetrue to address
		assert(read == valuetrue);
		assert(read2 != valuefalse);
	} else {
		//false branch should have written valuefalse to address
		assert(read != valuetrue);
		assert(read2 == valuefalse);
	}

	sd_uc_close(system);
}

void test_x86_compile_tac_if_cmp_goto_case_eq_true_8bit() {

	status_test_x86_codegen("TAC_IF_CMP_GOTO == true (8 bit)");

	common(1, TAC_OP_CMP_EQ, 1, true, false);
}

void test_x86_compile_tac_if_cmp_goto_case_eq_false_8bit() {

	status_test_x86_codegen("TAC_IF_CMP_GOTO == false (8 bit)");

	common(1, TAC_OP_CMP_EQ, 2, false, false);
}

void test_x86_compile_tac_if_cmp_goto_case_neq_true_8bit() {

	status_test_x86_codegen("TAC_IF_CMP_GOTO != true (8 bit)");

	common(1, TAC_OP_CMP_NEQ, 2, true, false);
}

void test_x86_compile_tac_if_cmp_goto_case_neq_false_8bit() {

	status_test_x86_codegen("TAC_IF_CMP_GOTO != false (8 bit)");

	common(1, TAC_OP_CMP_NEQ, 1, false, false);
}

void test_x86_compile_tac_if_cmp_goto_case_lt_true_8bit() {

	status_test_x86_codegen("TAC_IF_CMP_GOTO < true (8 bit)");

	common(1, TAC_OP_CMP_LT, 4, true, false);
}

void test_x86_compile_tac_if_cmp_goto_case_lt_false_8bit() {

	status_test_x86_codegen("TAC_IF_CMP_GOTO < false (8 bit)");

	common(5, TAC_OP_CMP_LT, 4, false, false);
}

void test_x86_compile_tac_if_cmp_goto_case_ge_true_8bit() {

	status_test_x86_codegen("TAC_IF_CMP_GOTO >= true (8 bit)");

	common(4, TAC_OP_CMP_GE, 4, true, false);
}

void test_x86_compile_tac_if_cmp_goto_case_ge_false_8bit() {

	status_test_x86_codegen("TAC_IF_CMP_GOTO >= false (8 bit)");

	common(2, TAC_OP_CMP_GE, 4, false, false);
}
