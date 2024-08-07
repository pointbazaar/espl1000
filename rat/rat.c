#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "../util/exit_malloc/exit_malloc.h"
#include "rat.h"

//gives index of a free register, exits on failure.
//the register still has to be occupied
static int rat_get_free_register(struct RAT* rat, bool high_regs_only, bool wide);

static bool rat_has_register(struct RAT* rat, uint32_t tmp_index);

static void rat_occupy(struct RAT* rat, uint8_t reg, uint32_t tmp_index, bool wide);

static void rat_init(struct RAT* rat);

enum RAT_REG_STATUS {
	REG_OCCUPIED, //reg occupied by temporary
	REG_RESERVED, //reg reserved, cannot be allocated
	REG_FREE, //reg can be allocated
};

struct RAT {
	//struct RAT should be opaque outside of it's
	//implementation file

	//Register Allocation Table
	//allocation of the registors used inside a function,
	//mapping temporaries to actual registers

	char* note[RAT_CAPACITY];
	uint32_t occupant[RAT_CAPACITY]; //who occupies it (which temp)
	enum RAT_REG_STATUS status[RAT_CAPACITY];

	//X: r26/r27
	//Y: r28/r29
	//Z: r30/r31
};

struct RAT* rat_ctor() {

	struct RAT* rat = exit_malloc(sizeof(struct RAT));

	for (int i = 0; i < RAT_CAPACITY; i++) {
		rat->status[i] = REG_FREE;
		rat->note[i] = "";
	}

	rat_init(rat);

	return rat;
}

static void rat_init(struct RAT* rat) {

	rat->status[0] = REG_RESERVED;
	rat->note[0] = "reserved for return value";
	rat->status[1] = REG_RESERVED;
	rat->note[1] = "reserved for return value";

	//r16 is another reserved multi-use register,
	//as there is a constraint that
	//many instructions can only use registers >= r16
	rat->status[RAT_SCRATCH_REG] = REG_RESERVED;
	rat->note[RAT_SCRATCH_REG] = "reserved as scratch register";

	//r26 through r31 are X,Y,Z
	//and are used as pointer registers,
	//and should not be available for allocation
	rat->status[26] = REG_RESERVED; //X
	rat->status[27] = REG_RESERVED; //X
	rat->note[26] = "XL";
	rat->note[27] = "XH";

	//Y is our base pointer for the stack frame
	rat->status[28] = REG_RESERVED; //Y
	rat->status[29] = REG_RESERVED; //Y
	rat->note[28] = "YL, reserved for frame pointer";
	rat->note[29] = "YH, reserved for frame pointer";

	rat->status[30] = REG_RESERVED; //Z
	rat->status[31] = REG_RESERVED; //Z
	rat->note[30] = "ZL";
	rat->note[31] = "ZH";
}

void rat_dtor(struct RAT* rat) {
	free(rat);
}

void rat_print(struct RAT* rat) {

	printf("Register Allocation Table:\n");
	for (size_t i = 0; i < RAT_CAPACITY; i++) {

		printf("r%02ld: ", i);

		switch (rat->status[i]) {

			case REG_OCCUPIED:
				printf("t%d", rat->occupant[i]);
				break;
			case REG_FREE:
				printf("%20s", " - ");
				break;
			case REG_RESERVED:
				printf("%20s", rat->note[i]);
				break;
		}

		printf("\n");
	}

	printf("------------\n");
}

static bool rat_has_register(struct RAT* rat, uint32_t tmp_index) {

	for (int i = 0; i < RAT_CAPACITY; i++) {
		if (rat->status[i] == REG_OCCUPIED && rat->occupant[i] == tmp_index) {
			return true;
		}
	}
	return false;
}

int rat_get_register(struct RAT* rat, uint32_t tmp_index) {

	for (int i = 0; i < RAT_CAPACITY; i++) {
		if (rat->status[i] == REG_OCCUPIED && (rat->occupant[i] == tmp_index)) {
			return i;
		}
	}

	printf("[RAT] rat_get_register: t%d has no register\n", tmp_index);
	rat_print(rat);
	exit(1);

	return -1;
}

uint32_t rat_occupant(struct RAT* rat, uint8_t reg) {

	if (rat->status[reg] != REG_OCCUPIED) {
		printf("[RAT] rat_occupant: requesting occupant for unoccupied register\n");
		rat_print(rat);
		exit(1);
	}

	return rat->occupant[reg];
}

static void rat_occupy(struct RAT* rat, uint8_t reg, uint32_t tmp_index, bool wide) {

	//occupy the register
	rat->status[reg] = REG_OCCUPIED;
	rat->occupant[reg] = tmp_index;

	if (wide) {
		rat->status[reg + 1] = REG_OCCUPIED;
		rat->occupant[reg + 1] = tmp_index;
	}
}

uint32_t rat_ensure_register(struct RAT* rat, uint32_t tmp_index, bool high_regs_only, bool wide) {

	//wide means we need a register pair, because of a 16 bit value

	//printf("rat ensure t%d, higher=%d",tmp_index, high_regs_only);
	//fflush(stdout);

	if (!rat_has_register(rat, tmp_index)) {

		uint32_t reg = rat_get_free_register(rat, high_regs_only, wide);

		rat_occupy(rat, reg, tmp_index, wide);
	}

	return rat_get_register(rat, tmp_index);
}

bool rat_is_wide(struct RAT* rat, uint32_t tmp_index) {

	//if the temporary occupies a register pair, return true

	int reg = rat_get_register(rat, tmp_index);

	return reg + 1 < RAT_CAPACITY && rat->status[reg + 1] == REG_OCCUPIED && (rat_occupant(rat, reg + 1) == tmp_index);
}

void rat_free(struct RAT* rat, uint8_t reg) {

	switch (rat->status[reg]) {

		case REG_OCCUPIED:
			rat->status[reg] = REG_FREE;
			break;

		case REG_FREE:
			printf("[RAT] double free. Exiting.\n");
			exit(1);
			break;

		case REG_RESERVED:
			printf("[RAT] trying to free reserved Register. Exiting.\n");
			exit(1);
			break;
	}
}

static int rat_get_free_register(struct RAT* rat, bool high_regs_only, bool wide) {

	//wide means we need a register pair.
	//we return the lower of the 2 registers

	//high_regs_only tells us at which register to start looking,
	//as there are some instructions such as 'ldi' which can only use a high register
	// (>= r16)

	const int start_inclusive = (high_regs_only) ? 16 : 0;

	for (int i = start_inclusive; i < RAT_CAPACITY; i++) {

		if (rat->status[i] == REG_FREE) {

			if (wide) {
				if (i + 1 < RAT_CAPACITY && rat->status[i + 1] == REG_FREE) {
					return i;
				}
			} else {
				return i;
			}
		}
	}

	printf("RAT could not find a free register, they are all occupied.\n");
	rat_print(rat);
	exit(1);
	return -1;
}
