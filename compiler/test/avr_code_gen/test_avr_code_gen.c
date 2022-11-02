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

void status_test_codegen(char* msg){
    printf(" - [TEST] avr codegen %s\n", msg);
}

void test_suite_avr_code_gen(){

    test_compile_tac_setup_sp();
    
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
    test_compile_tac_setup_stackframe();
    
    test_compile_tac_load();
    test_compile_tac_store();
}

