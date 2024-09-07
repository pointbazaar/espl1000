#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "avr_code_gen/cg_avr.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "test_avr_code_gen.h"
#include "test_avr_code_gen_util.h"

#include "compile_ir/test_compile_tac.h"

#include "timer/test_avr_code_gen_timer.h"

void status_test_codegen(char* msg) {
	printf(" - [TEST] avr codegen %s\n", msg);
}
