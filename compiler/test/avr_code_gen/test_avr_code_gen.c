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

//unit tests forward declarations
static void test_stack_pointer_setup_correctly();

void status_test_codegen(char* msg){
    printf(" - [TEST] avr codegen %s\n", msg);
}

void test_suite_avr_code_gen(){

    test_stack_pointer_setup_correctly();
    //test_reaches_endloop();
    
    test_compile_tac_nop();
    test_compile_tac_const_value();
    
    test_compile_tac_store_const_addr();
    test_compile_tac_load_const_addr();
    
    test_compile_tac_binary_op_immediate();
    test_compile_tac_unary_op();
    test_compile_tac_binary_op();
    
    test_compile_tac_goto();
    test_compile_tac_if_goto();
    test_compile_tac_if_cmp_goto();
    
    test_compile_tac_return();
    test_compile_tac_copy();
    test_compile_tac_param();
    test_compile_tac_call();
    
    test_compile_tac_load();
    test_compile_tac_store();
}

// --- UNIT TESTS ---

static void test_stack_pointer_setup_correctly(){

    status_test_codegen("stackpointer setup correctly");

    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACSetupSP());
    tacbuffer_append(buffer, makeTACNop());

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    vmcu_system_step_n(system, 4);

    uint16_t sp = vmcu_system_read_sp(system);

    assert(sp == 0x085f);

    vmcu_system_dtor(system);
}
