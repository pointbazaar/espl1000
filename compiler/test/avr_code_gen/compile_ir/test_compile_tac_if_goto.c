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

void test_compile_tac_if_goto_case_true_8bit() {

	status_test_codegen("TAC_IF_GOTO true (8 bit)");

	const int8_t value = 0x33 | 0x1;
	const uint16_t address1 = 0x105;

	//labels
	const uint16_t l1 = 1;
	const uint16_t lend = 2;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, 1));
	tacbuffer_append(b, makeTACIfGoto(1, l1));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(l1));
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address1, 0));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACGoto(lend));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 13);

	assert(vmcu_system_read_data(system, address1) == value);

	vmcu_system_dtor(system);
}

void test_compile_tac_if_goto_case_true_16bit() {

	status_test_codegen("TAC_IF_GOTO true (16 bit)");

	const int8_t value = 0x33 | 0x1;
	const uint16_t address1 = 0x101;

	//labels
	const uint16_t l1 = 1;
	const uint16_t lend = 2;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, 0x0101));
	tacbuffer_append(b, makeTACIfGoto(1, l1));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(l1));
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address1, 0));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACGoto(lend));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 13);

	assert(vmcu_system_read_data(system, address1) == value);

	vmcu_system_dtor(system);
}

void test_compile_tac_if_goto_case_false_8bit() {

	status_test_codegen("TAC_IF_GOTO false (8 bit)");

	const int8_t value = 0x22 | 0x1;
	const uint16_t address1 = 0x108;

	//labels
	const uint16_t l1 = 1;
	const uint16_t lend = 2;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, 0));
	tacbuffer_append(b, makeTACIfGoto(1, l1));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(l1));
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address1, 0));

	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACGoto(lend));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 13);

	assert(vmcu_system_read_data(system, address1) != value);

	vmcu_system_dtor(system);
}

void test_compile_tac_if_goto_case_false_16bit() {

	status_test_codegen("TAC_IF_GOTO false (16 bit)");

	const int8_t value = 0x11 | 0x1;
	const uint16_t address1 = 0x109;

	//labels
	const uint16_t l1 = 1;
	const uint16_t lend = 2;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(1, 0x0000));
	tacbuffer_append(b, makeTACIfGoto(1, l1));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(l1));
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address1, 0));

	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACGoto(lend));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 13);

	assert(vmcu_system_read_data(system, address1) != value);

	vmcu_system_dtor(system);
}

void test_compile_tac_if_goto_case_mixed() {

	status_test_codegen("TAC_IF_GOTO mixed");

	//we need to test 2 cases
	// - condition true, we branch
	// - condition false, we do not branch
	//we check by writing a fixed value to 2 known addresses in data space

	const int8_t value = 0xab | 0x1;
	const uint16_t address1 = 0x10f;
	const uint16_t address2 = address1 + 1;

	//labels
	const uint16_t l1 = 1;
	const uint16_t lend = 2;

	struct TACBuffer* b = tacbuffer_ctor();

	tacbuffer_append(b, makeTACConst(0, value));

	tacbuffer_append(b, makeTACConst(1, 1));
	tacbuffer_append(b, makeTACIfGoto(1, l1)); //should branch
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(l1));
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address1, 0));
	tacbuffer_append(b, makeTACConst(2, 0));
	tacbuffer_append(b, makeTACIfGoto(2, lend)); //should not branch
	tacbuffer_append(b, makeTACConst(0, value));
	tacbuffer_append(b, makeTACStoreConstAddr(address2, 0));
	tacbuffer_append(b, makeTACGoto(lend));

	tacbuffer_append(b, makeTACLabel(lend));
	tacbuffer_append(b, makeTACGoto(lend));

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	vmcu_system_step_n(system, 20);

	//check that the values have arrived
	assert(vmcu_system_read_data(system, address1) == value);
	assert(vmcu_system_read_data(system, address2) == value);

	vmcu_system_dtor(system);
}
