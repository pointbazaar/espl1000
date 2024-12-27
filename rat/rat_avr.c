#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/exit_malloc/exit_malloc.h"
#include "rat.h"
#include "register.h"
#include "_struct.h"

void rat_init_avr(struct RAT* rat) {

	for (uint32_t reg = r0; reg <= r31; reg++) {
		rat->status[reg] = REG_FREE;
	}

	rat_reserve_reg(rat, rat_return_reg(rat), "reserved for return value");
	rat_reserve_reg(rat, rat_return_reg(rat) + 1, "reserved for return value");
	rat_reserve_reg(rat, rat_scratch_reg(rat), "reserved as scratch register");

	//r26 through r31 are X,Y,Z
	//and are used as pointer registers,
	//and should not be available for allocation
	rat_reserve_reg(rat, r26, "XL");
	rat_reserve_reg(rat, r27, "XH");

	//Y is our base pointer for the stack frame
	rat_reserve_reg(rat, rat_base_ptr(rat), "YL, frame ptr");
	rat_reserve_reg(rat, rat_base_ptr(rat) + 1, "YH, frame ptr");

	rat_reserve_reg(rat, r30, "ZL");
	rat_reserve_reg(rat, r31, "ZH");
}
