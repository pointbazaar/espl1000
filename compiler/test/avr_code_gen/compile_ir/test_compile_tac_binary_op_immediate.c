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

void test_compile_tac_binary_op_immediate(){

	status_test_codegen("TAC_BINARY_OP_IMMEDIATE");
	
	//we test +,-,|,& operators
	uint8_t change = 40;
	
	int8_t starts[] = {rand()%100,rand()%100,rand()%100,rand()%100};
	int8_t changes[] = {rand()%change,rand()%change,rand()%change,rand()%change};
	int8_t expected[] = {
		starts[0] + changes[0],
		starts[1] - changes[1],
		starts[2] | changes[2],
		starts[3] & changes[3],
	};
	
	char* temps[] = {"t0","t1","t2","t3"};
	enum TAC_OP ops[] = {TAC_OP_ADD, TAC_OP_SUB, TAC_OP_OR, TAC_OP_AND};

    struct TACBuffer* buffer = tacbuffer_ctor();
    
    //add all the operations
    for(int i=0; i < 4; i++){
		
		tacbuffer_append(buffer, makeTACConst(i, starts[i]));
		tacbuffer_append(buffer, makeTACBinOpImmediate(temps[i], ops[i], changes[i]));
	}
    
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

	//keep track of which values have initialized and which changed
	bool has_init[]    = {false,false,false,false};
	bool has_changed[] = {false,false,false,false};

    for(int i=0;i < 20; i++){
		
        vmcu_system_step(system);
        
        for(int k = 0; k < 32; k++){
			
			const int8_t value = vmcu_system_read_gpr(system, k);
			
			for(int j = 0; j < 4; j++){
				
				if(has_init[j] && value == expected[j]) 
					has_changed[j] = true;
			
				if(value == starts[j]) 
					has_init[j] = true;
			}
		}
	}
	
	for(int j = 1; j < 2; j++){
		
		assert(has_init[j]);
		assert(has_changed[j]);
	}
	
	vmcu_system_dtor(system);
}
