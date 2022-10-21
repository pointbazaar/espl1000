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


void test_compile_tac_unary_op(){
	
	status_test_codegen("TAC_UNARY_OP");
    
	const int8_t start = rand()%0xff;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
	tacbuffer_append(buffer, makeTACConst(0, start));
	
	tacbuffer_append(buffer, makeTACUnaryOp("t1","t0",TAC_OP_UNARY_MINUS));
    tacbuffer_append(buffer, makeTACUnaryOp("t2","t0",TAC_OP_UNARY_NOT));
    tacbuffer_append(buffer, makeTACUnaryOp("t3","t0",TAC_OP_UNARY_BITWISE_NEG));
    
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);
    
    bool has1 = false;
	bool has2 = false;
	bool has3 = false;

    for(int i=0;i < 16; i++){
		
        vmcu_system_step(system);
        
        for(int k = 0; k < 32; k++){
			
			const int8_t value = vmcu_system_read_gpr(system, k);
			
			if(value == -start) has1 = true;
			if(value == !start) has2 = true;
			if(value == ~start) has3 = true;
		}
	}
	
	assert(has1);
	assert(has2);
	assert(has3);
	
	vmcu_system_dtor(system);
}
