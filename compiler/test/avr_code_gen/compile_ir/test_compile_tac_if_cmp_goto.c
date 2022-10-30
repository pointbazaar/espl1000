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

static void case_eq_true();
static void case_eq_false();
static void case_neq_true();
static void case_neq_false();
static void case_lt_true();
static void case_lt_false();
static void case_ge_true();
static void case_ge_false();

void test_compile_tac_if_cmp_goto(){
	
	case_eq_true();
	case_eq_false();
	case_neq_true();
	case_neq_false();
	case_lt_true();
	case_lt_false();
	case_ge_true();
	case_ge_false();
}

static void case_eq_true(){
	
	status_test_codegen("TAC_IF_CMP_GOTO == true");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(1, 1));
    tacbuffer_append(buffer, makeTACConst(2, 1));
    tacbuffer_append(buffer, makeTACIfCMPGoto(1, TAC_OP_CMP_EQ, 2, l1));
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, 0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	assert(vmcu_system_read_data(system, address1) == value);
	
	vmcu_system_dtor(system);
}

static void case_eq_false(){
	
	status_test_codegen("TAC_IF_CMP_GOTO == false");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(1, 1));
    tacbuffer_append(buffer, makeTACConst(2, 2));
    tacbuffer_append(buffer, makeTACIfCMPGoto(1, TAC_OP_CMP_EQ, 2, l1));
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, 0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	assert(vmcu_system_read_data(system, address1) != value);
	
	vmcu_system_dtor(system);
}

static void case_neq_true(){
	
	status_test_codegen("TAC_IF_CMP_GOTO != true");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(1, 3));
    tacbuffer_append(buffer, makeTACConst(2, 1));
    tacbuffer_append(buffer, makeTACIfCMPGoto(1, TAC_OP_CMP_NEQ, 2, l1));
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, 0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	assert(vmcu_system_read_data(system, address1) == value);
	
	vmcu_system_dtor(system);
}

static void case_neq_false(){
	
	status_test_codegen("TAC_IF_CMP_GOTO != false");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(1, 3));
    tacbuffer_append(buffer, makeTACConst(2, 3));
    tacbuffer_append(buffer, makeTACIfCMPGoto(1, TAC_OP_CMP_NEQ, 2, l1));
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, 0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	assert(vmcu_system_read_data(system, address1) != value);
	
	vmcu_system_dtor(system);
}

static void case_lt_true(){
	
	status_test_codegen("TAC_IF_CMP_GOTO < true");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(1, 1));
    tacbuffer_append(buffer, makeTACConst(2, 4));
    tacbuffer_append(buffer, makeTACIfCMPGoto(1, TAC_OP_CMP_LT, 2, l1));
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, 0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	assert(vmcu_system_read_data(system, address1) == value);
	
	vmcu_system_dtor(system);
}

static void case_lt_false(){
	
	status_test_codegen("TAC_IF_CMP_GOTO < false");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(1, 5));
    tacbuffer_append(buffer, makeTACConst(2, 4));
    tacbuffer_append(buffer, makeTACIfCMPGoto(1, TAC_OP_CMP_LT, 2, l1));
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, 0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	assert(vmcu_system_read_data(system, address1) != value);
	
	vmcu_system_dtor(system);
}

static void case_ge_true(){
	
	status_test_codegen("TAC_IF_CMP_GOTO >= true");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(1, 4));
    tacbuffer_append(buffer, makeTACConst(2, 4));
    tacbuffer_append(buffer, makeTACIfCMPGoto(1, TAC_OP_CMP_GE, 2, l1));
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, 0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	assert(vmcu_system_read_data(system, address1) == value);
	
	vmcu_system_dtor(system);
}

static void case_ge_false(){
	
	status_test_codegen("TAC_IF_CMP_GOTO >= false");
	
	const int8_t value      = rand()%0xff;
	const uint16_t address1 = 0x100+rand()%30;
	
	const uint16_t l1   = 1;
	const uint16_t lend = 2;
	
    struct TACBuffer* buffer = tacbuffer_ctor();
    
    tacbuffer_append(buffer, makeTACConst(1, 2));
    tacbuffer_append(buffer, makeTACConst(2, 4));
    tacbuffer_append(buffer, makeTACIfCMPGoto(1, TAC_OP_CMP_GE, 2, l1));
	tacbuffer_append(buffer, makeTACGoto(lend));
	
	tacbuffer_append(buffer, makeTACLabel(l1));
	tacbuffer_append(buffer, makeTACConst(0, value));
	tacbuffer_append(buffer, makeTACStoreConstAddr(address1, "t0"));
	
	tacbuffer_append(buffer, makeTACLabel(lend));
	tacbuffer_append(buffer, makeTACConst(0, 0));
    tacbuffer_append(buffer, makeTACReturn("t0"));

    vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(buffer);

    for(int i=0;i < 30; i++){
        vmcu_system_step(system);
	}
	
	assert(vmcu_system_read_data(system, address1) != value);
	
	vmcu_system_dtor(system);
}
