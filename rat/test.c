#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "rat.h"

#include "test.h"

#include "util/status/status.h"

void test_rat_scratch_reg() {

	status("scratch register");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

	assert(rat_scratch_reg(rat) == 16);

	rat_dtor(rat);
}

void test_rat_alloc_different_regs() {

	status("allocate different registers");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

	int reg1 = rat_ensure_register(rat, 0, false, false);

	int reg2 = rat_ensure_register(rat, 1, false, false);

	assert(reg1 != reg2);

	rat_dtor(rat);
}

void test_rat_alloc_in_bounds() {

	status("allocate in bounds");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

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

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

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

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

	const int reg1 = rat_ensure_register(rat, 342, false, false);

	assert(rat_occupant(rat, reg1) == 342);

	rat_dtor(rat);
}

void test_rat_can_free_pair() {

	status("can free a register pair");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

	const int reg1 = rat_ensure_register(rat, 0, true, true);

	rat_free(rat, reg1);
	rat_free(rat, reg1 + 1);

	const int reg2 = rat_ensure_register(rat, 0, true, true);

	assert(reg1 == reg2);

	rat_dtor(rat);
}

void test_rat_occupant_pair() {

	status("can occupy and read occupant for pair");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

	const int reg1 = rat_ensure_register(rat, 342, false, true);

	assert(rat_occupant(rat, reg1) == 342);
	assert(rat_occupant(rat, reg1 + 1) == 342);

	rat_dtor(rat);
}

void test_rat_alloc_n_regs() {

	status("can allocate a number of regs without running out");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

	for (int i = 0; i < 14; i++) {

		rat_ensure_register(rat, i, false, false);
	}

	rat_dtor(rat);
}

void test_rat_is_wide() {

	status("rat_is_wide(...)");

	struct RAT* rat = rat_ctor(RAT_ARCH_AVR);

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
