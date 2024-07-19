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

void test_compile_tac_setup_stackframe() {

	//see if stackpointer is setup correctly
	status_test_codegen("TAC_SETUP_STACKFRAME");

	for (uint8_t size = 0; size < 10; size++) {

		struct TACBuffer* buffer = tacbuffer_ctor();

		tacbuffer_append(buffer, makeTACSetupSP());
		tacbuffer_append(buffer, makeTACSetupStackframe(size));

		vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

		vmcu_system_step_n(system, 4); //init SP

		uint16_t sp_old = vmcu_system_read_sp(system);

		vmcu_system_step_n(system, size); //setup stackframe
		vmcu_system_step_n(system, 2);

		//assert that Y + size == SP_old after setup stackframe
		uint16_t y = vmcu_system_read_y(system);

		assert(y + size == sp_old);

		assert(y == vmcu_system_read_sp(system));

		vmcu_system_dtor(system);
	}
}
