#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "libvmcu/libvmcu_system.h"
#include "libvmcu/libvmcu_analyzer.h"

#include "compiler/test/libvmcu_utils/libvmcu_utils.h"

#include "avr_code_gen/cg_avr.h"
#include "avr_code_gen/cg_avr_basic_block.h"

#include "tac/tacbuffer.h"

#include "../test_avr_code_gen.h"
#include "../test_avr_code_gen_util.h"

#include "test_avr_code_gen_timer.h"

#define append(tac) tacbuffer_append(b, tac)

//defining some addresses of I/O Registers

const uint16_t TCCR0A = 0x44;
const uint16_t TCCR0B = 0x45;
const uint16_t TIMSK0 = 0x6e;
const uint16_t TCNT0 = 0x46;

const uint16_t TCCR1A = 0x80;
const uint16_t TCNT1L = 0x84;
const uint16_t TCNT1H = 0x85;
const uint16_t TCCR1B = 0x05;
const uint16_t TIFR1 = 0x36;

static void test_timer0();
static void test_timer1();

void test_avr_code_gen_timer() {

	test_timer0();
	//test_timer1();
}

// Testing AVR Codegen relating to the internal timers.

static void test_timer0() {

	status_test_codegen("Timer0 Test");

	// Initialize the Timer and wait for overflow

	struct TACBuffer* b = tacbuffer_ctor();

	//general timer0 setup
	append(makeTACConst(1, 0x0));
	append(makeTACStoreConstAddr(TCCR0A, 1));

	//clock prescaler select, no prescaler,
	//so timer0 should increment with full clock speed
	append(makeTACConst(1, 0b00000001));
	append(makeTACStoreConstAddr(TCCR0B, 1));

	//timer should start working now

#define read(addr) vmcu_system_read_data(system, addr)

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	//wait for timer to be setup
	uint8_t initial = read(TCNT0);

	for (int i = 0; i < 20; i++) {
		vmcu_system_step_n(system, 1);
		if (read(TCNT0) != initial) break;
	}

	assert(read(TCNT0) == 2);

	vmcu_system_step_n(system, 1);
	assert(read(TCNT0) == 3);

	vmcu_system_step_n(system, 1);

	assert(read(TCNT0) == 4);
	vmcu_system_step_n(system, 1);

	for (int i = 0; i < 100; i++) {

		assert(read(TCNT0) == i + 5);
		vmcu_system_step_n(system, 1);
	}

	vmcu_system_dtor(system);
}

static void test_timer1() {

	status_test_codegen("Timer1 Test");

	// Initialize the Timer and wait for overflow

	struct TACBuffer* b = tacbuffer_ctor();

	append(makeTACConst(1, 0x0));
	append(makeTACStoreConstAddr(TCCR1A, 1));

	//important to write the high byte before the low byte,
	//see ATMega328P Datasheet
	append(makeTACConst(1, 0xc2));
	append(makeTACStoreConstAddr(TCNT1H, 1));

	append(makeTACConst(1, 0xf7));
	append(makeTACStoreConstAddr(TCNT1L, 1));

	append(makeTACConst(1, 0x05));
	append(makeTACStoreConstAddr(TCCR1B, 1));

	//timer should start working now

	vmcu_system_t* system = prepare_vmcu_system_from_tacbuffer(b);

	for (int i = 0; i < 100; i++) {
		vmcu_system_step_n(system, 1);

		const uint8_t tifr1 = vmcu_system_read_data(system, TIFR1);
		const uint8_t tcnt1l = vmcu_system_read_data(system, TCNT1L);
		const uint8_t tcnt1h = vmcu_system_read_data(system, TCNT1H);

		const uint16_t tcnt1 = (tcnt1h << 8) | tcnt1l;

		printf("TIFR1=0x%x, TCNT1=0x%x\n", tifr1, tcnt1);
	}

	//TODO: assert something
	//assert(vmcu_system_read_gpr(system, 0) == fixed_value);

	vmcu_system_dtor(system);
}
