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

void test_compile_tac_store_const_addr(){

	status_test_codegen("TAC_STORE_CONST_ADDR");

	for(uint16_t addr = 0x118; addr < 0x140; addr++){

		const int8_t fixed_value = 0x44;

		struct TACBuffer* b = tacbuffer_ctor();

		tacbuffer_append(b, makeTACConst(1, fixed_value));
		tacbuffer_append(b, makeTACStoreConstAddr(addr, 1));
		tacbuffer_append(b, makeTACReturn(1));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

		vmcu_system_step_n(system, 10);

		//check that the value was written

		int8_t value = vmcu_system_read_data(system, addr);

		assert(value == fixed_value);

		vmcu_system_dtor(system);
	}
}
