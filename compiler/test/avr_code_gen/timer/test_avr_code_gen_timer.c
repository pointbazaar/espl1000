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

void test_avr_code_gen_timer() {

	test_timer0();
}

// Testing AVR Codegen relating to the internal timers.

static void test_timer0() {

	status_test_codegen("Timer0 Test");

	// Initialize the Timer and wait for overflow

	struct TACBuffer* b = tacbuffer_ctor();

	//general timer0 setup
	append(makeTACConst(0, 1, 0x0));

	append(makeTACConst(0, 2, TCCR0A));
	append(makeTACStore(0, 2, 1, 1));

	//clock prescaler select, no prescaler,
	//so timer0 should increment with full clock speed
	append(makeTACConst(0, 1, 0b00000001));

	append(makeTACConst(0, 2, TCCR0B));
	append(makeTACStore(0, 2, 1, 1));

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
