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

//unit tests forward declarations
static void test_stack_pointer_setup_correctly();
static void test_reaches_endloop();
/*
static void test_call_and_return();
static void test_write_memory();
static void test_read_memory();
 */

static void status_test_codegen(char* msg){
    printf(" - [TEST] avr codegen %s\n", msg);
}

void test_suite_avr_code_gen(){

    test_stack_pointer_setup_correctly();
    test_reaches_endloop();
    //TODO
}

// --- UNIT TESTS ---

static void test_stack_pointer_setup_correctly(){

    status_test_codegen("stackpointer setup correctly");

    struct TACBuffer* buffer = tacbuffer_ctor();

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    //step 4 times
    for(int i=0;i < 4;i++)
        vmcu_system_step(system);

    //assert SPH:SPL == 0x085f

    //the 'out' instruction receives an offset to the base address off the io space within the data space
    const uint32_t io_offset = 0x20;
    int8_t SPH_value = vmcu_system_read_data(system, io_offset+0x3e);
    int8_t SPL_value = vmcu_system_read_data(system, io_offset+0x3d);

    //printf("SPH = %04x, SPL = %04x\n", SPH_value, SPL_value);

    assert((SPH_value << 8 | SPL_value) == 0x085f);

    vmcu_system_dtor(system);
}

static void test_reaches_endloop(){

    status_test_codegen("reaches endloop");

    //create a TACBuffer* where we put the TAC that in the end will be compiled

    struct TACBuffer* buffer = tacbuffer_ctor();
    buffer->count = 2;

    struct TAC* tconst = makeTACConst(1, 0);
    struct TAC* treturn = makeTACReturn("t1");

    buffer->buffer[0] = tconst;
    buffer->buffer[1] = treturn;

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    //step through the system and see that it should converge on a value for pc (endloop)

    for(int i=0;i < 30; i++)
        vmcu_system_step(system);

    int32_t pc1 = (int32_t)vmcu_system_get_pc(system);
    vmcu_system_step(system);
    int32_t pc2 = (int32_t)vmcu_system_get_pc(system);

    //check that the difference is 1
    assert(pc1 == pc2);

    //check that pc remains in range
    for(int i=0;i < 10; i++) {

        vmcu_system_step(system);
        int32_t pc = (int32_t)vmcu_system_get_pc(system);

        assert(pc == pc1);
    }

    vmcu_system_dtor(system);
}