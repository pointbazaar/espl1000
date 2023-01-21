#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "avr_code_gen/cg_avr.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_avr_code_gen.h"
#include "../test_avr_code_gen_util.h"

#include "test_compile_tac.h"

static void case_eq_true_8bit();
static void case_eq_true_16bit();
static void case_eq_false_8bit();
static void case_eq_false_16bit();
static void case_neq_true_8bit();
static void case_neq_true_16bit();
static void case_neq_false_8bit();
static void case_neq_false_16bit();
static void case_lt_true_8bit();
static void case_lt_true_16bit();
static void case_lt_false_8bit();
static void case_lt_false_16bit();
static void case_ge_true_8bit();
static void case_ge_true_16bit();
static void case_ge_false_8bit();
static void case_ge_false_16bit();

static void common(int a, enum TAC_OP op, int b, bool expect_true);

void test_compile_tac_if_cmp_goto(){

	case_eq_true_8bit();
	case_eq_true_16bit();
	case_eq_false_8bit();
	case_eq_false_16bit();

	case_neq_true_8bit();
	case_neq_true_16bit();
	case_neq_false_8bit();
	case_neq_false_16bit();

	case_lt_true_8bit();
	case_lt_true_16bit();
	case_lt_false_8bit();
	case_lt_false_16bit();

	case_ge_true_8bit();
	case_ge_true_16bit();
	case_ge_false_8bit();
	case_ge_false_16bit();
}

static void common(int a1, enum TAC_OP op, int a2, bool expect_true){

	const int8_t valuetrue  = rand()%0xff;
	const int8_t valuefalse = valuetrue + 1;
	const uint16_t address  = 0x100+rand()%30;

	const uint16_t ltrue  = 1;
	const uint16_t lfalse = 2;
	const uint16_t lend   = 3;

	struct TACBuffer* b = tacbuffer_ctor();

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
	tacbuffer_append(b, makeTACStoreConstAddr(address+1, 0));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACGoto(lend));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	assert(vmcu_system_read_data(system, address)   == 0x00);
	assert(vmcu_system_read_data(system, address+1) == 0x00);

	vmcu_system_step_n(system, 13);

	if(expect_true){
		//true branch should have written valuetrue to address
		assert(vmcu_system_read_data(system, address)   == valuetrue);
		assert(vmcu_system_read_data(system, address+1) != valuefalse);
	}else{
		//false branch should have written valuefalse to address
		assert(vmcu_system_read_data(system, address)   != valuetrue);
		assert(vmcu_system_read_data(system, address+1) == valuefalse);
	}

	vmcu_system_dtor(system);
}

static void case_eq_true_8bit(){

	status_test_codegen("TAC_IF_CMP_GOTO == true (8 bit)");

	common(1, TAC_OP_CMP_EQ, 1, true);
}

static void case_eq_true_16bit(){

	status_test_codegen("TAC_IF_CMP_GOTO == true (16 bit)");

	common(0xffff, TAC_OP_CMP_EQ, 0xffff, true);
}

static void case_eq_false_8bit(){

	status_test_codegen("TAC_IF_CMP_GOTO == false (8 bit)");

	common(1, TAC_OP_CMP_EQ, 2, false);
}

static void case_eq_false_16bit(){

	status_test_codegen("TAC_IF_CMP_GOTO == false (16 bit)");

	common(0x0100, TAC_OP_CMP_EQ, 0x0200, false);
}

static void case_neq_true_8bit(){

	status_test_codegen("TAC_IF_CMP_GOTO != true (8 bit)");

	common(1, TAC_OP_CMP_NEQ, 2, true);
}

static void case_neq_true_16bit(){

	status_test_codegen("TAC_IF_CMP_GOTO != true (16 bit)");

	common(0x0100, TAC_OP_CMP_NEQ, 0x0200, true);
}

static void case_neq_false_8bit(){

	status_test_codegen("TAC_IF_CMP_GOTO != false (8 bit)");

	common(1, TAC_OP_CMP_NEQ, 1, false);
}

static void case_neq_false_16bit(){

	status_test_codegen("TAC_IF_CMP_GOTO != false (16 bit)");

	common(0x1234, TAC_OP_CMP_NEQ, 0x1234, false);
}

static void case_lt_true_8bit(){

	status_test_codegen("TAC_IF_CMP_GOTO < true (8 bit)");

	common(1, TAC_OP_CMP_LT, 4, true);
}

static void case_lt_true_16bit(){

	status_test_codegen("TAC_IF_CMP_GOTO < true (16 bit)");

	common(0x0100, TAC_OP_CMP_LT, 0x0200, true);
}

static void case_lt_false_8bit(){

	status_test_codegen("TAC_IF_CMP_GOTO < false (8 bit)");

	common(5, TAC_OP_CMP_LT, 4, false);
}

static void case_lt_false_16bit(){

	status_test_codegen("TAC_IF_CMP_GOTO < false (16 bit)");

	common(0x0100, TAC_OP_CMP_LT, 0x0001, false);
}

static void case_ge_true_8bit(){

	status_test_codegen("TAC_IF_CMP_GOTO >= true (8 bit)");

	common(4, TAC_OP_CMP_GE, 4, true);
}

static void case_ge_true_16bit(){

	status_test_codegen("TAC_IF_CMP_GOTO >= true (16 bit)");

	common(0x0200, TAC_OP_CMP_GE, 0x0101, true);
}

static void case_ge_false_8bit(){

	status_test_codegen("TAC_IF_CMP_GOTO >= false (8 bit)");

	common(2, TAC_OP_CMP_GE, 4, false);
}

static void case_ge_false_16bit(){

	status_test_codegen("TAC_IF_CMP_GOTO >= false (16 bit)");

	common(0x0101, TAC_OP_CMP_GE, 0x0200, false);
}
