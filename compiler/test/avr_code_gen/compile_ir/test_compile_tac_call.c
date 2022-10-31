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

static void case_recurse();
static void case_returns_value();
static void case_1_param();

void test_compile_tac_call(){

	case_recurse();
	case_returns_value();
	case_1_param();
}

static void case_recurse(){
	
	status_test_codegen("TAC_CALL - recurse");
	
	const int8_t value = rand()%0xff;
	
    struct TACBuffer* b = tacbuffer_ctor();
    
    tacbuffer_append(b, makeTACLabelFunction(0));
    tacbuffer_append(b, makeTACConst(0, value));
    
    //tacbuffer_append(b, makeTACCall(1, "main"));
    tacbuffer_append(b, makeTACCall(1, 0));
    
    tacbuffer_append(b, makeTACReturn(0));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

    for(int i=0;i < 40; i++){
        vmcu_system_step(system);
        
        //r0 should never have that value, as we are never reaching
        //the return, always recursing
        assert(vmcu_system_read_gpr(system, 0) != value);
	}
	
	vmcu_system_dtor(system);
}

static void case_returns_value(){
	
	status_test_codegen("TAC_CALL - returns value [TODO]");
}

static void case_1_param(){
	
	status_test_codegen("TAC_CALL - 1 param [TODO]");
}
