#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "rat/rat.h"

#include "test.h"

#include "util/status/status.h"

void test_rat_scratch_reg() {

	status("scratch register");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 1);

	assert(rat_scratch_reg(rat) == r16);

	rat_dtor(rat);
}

void test_rat_alloc_different_regs() {

	status("allocate different registers");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 2);

	int reg1 = rat_ensure_register(rat, 0, false, false);

	int reg2 = rat_ensure_register(rat, 1, false, false);

	assert(reg1 != reg2);

	rat_dtor(rat);
}

void test_rat_alloc_in_bounds() {

	status("allocate in bounds");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 2);

	int reg1 = rat_ensure_register(rat, 0, false, false);

	assert(reg1 >= 0);
	assert(reg1 < 32);

	int reg2 = rat_ensure_register(rat, 1, true, false);

	assert(reg2 < 32);
	assert(reg2 >= 16);

	rat_dtor(rat);
}

void test_rat_can_free() {

	status("can free a register");

	//test that we can give a register back when it's no longer needed
	//and then get it back later on

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 2);

	const int reg1 = rat_ensure_register(rat, 0, true, false);

	rat_free(rat, reg1);

	const int reg2 = rat_ensure_register(rat, 0, true, false);

	assert(reg1 == reg2);

	rat_dtor(rat);
}

void test_rat_occupant() {

	status("can occupy and read occupant");

	//test that a temp can occupy a register and we can find out which
	//temp it is

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 343);

	const int reg1 = rat_ensure_register(rat, 342, false, false);

	assert(rat_occupant(rat, reg1) == 342);

	rat_dtor(rat);
}

void test_rat_can_free_pair() {

	status("can free a register pair");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 2);

	const int reg1 = rat_ensure_register(rat, 0, true, true);

	rat_free(rat, reg1);
	rat_free(rat, reg1 + 1);

	const int reg2 = rat_ensure_register(rat, 0, true, true);

	assert(reg1 == reg2);

	rat_dtor(rat);
}

void test_rat_occupant_pair() {

	status("can occupy and read occupant for pair");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 35);

	const int reg1 = rat_ensure_register(rat, 34, false, true);

	assert(rat_occupant(rat, reg1) == 34);
	assert(rat_occupant(rat, reg1 + 1) == 34);

	rat_dtor(rat);
}

void test_rat_alloc_n_regs() {

	status("can allocate a number of regs without running out");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 15);

	for (int i = 0; i < 14; i++) {

		rat_ensure_register(rat, i, false, false);
	}

	rat_dtor(rat);
}

void test_rat_is_wide() {

	status("rat_is_wide(...)");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 2);

	//with low regs
	rat_ensure_register(rat, 0, false, false);
	assert(rat_is_wide(rat, 0) == false);

	rat_ensure_register(rat, 1, false, true);
	assert(rat_is_wide(rat, 1) == true);

	//with high regs
	rat_ensure_register(rat, 0, true, false);
	assert(rat_is_wide(rat, 0) == false);

	rat_ensure_register(rat, 1, true, true);
	assert(rat_is_wide(rat, 1) == true);

	rat_dtor(rat);
}

void test_rat_multiple_temps_in_register() {

	status("rat: multiple temporaries in a register");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR, 2);

	assert(rat_occupies(rat, 0, 0) == false);
	assert(rat_occupies(rat, 0, 1) == false);

	rat_occupy(rat, 0, 0, false);

	assert(rat_occupies(rat, 0, 0) == true);
	assert(rat_occupies(rat, 0, 1) == false);

	rat_occupy(rat, 0, 1, false);

	assert(rat_occupies(rat, 0, 0) == true);
	assert(rat_occupies(rat, 0, 1) == true);

	rat_dtor(rat);
}
