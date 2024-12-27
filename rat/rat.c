#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/exit_malloc/exit_malloc.h"
#include "rat.h"
#include "_struct.h"

//gives index of a free register, exits on failure.
//the register still has to be occupied
static int rat_get_free_register(struct RAT* rat, bool high_regs_only, bool wide);

static void rat_init(struct RAT* rat);
static void rat_init_avr(struct RAT* rat);
static void rat_init_x86(struct RAT* rat);

// @param arch    the target architecture
// @param ntemps  maximum number of temporaries
//                to make space for
struct RAT* rat_ctor(enum RAT_ARCH arch, size_t ntemps) {

	struct RAT* rat = exit_malloc(sizeof(struct RAT));

	rat->arch = arch;
	rat->ntemps = ntemps;

	for (int i = 0; i < rat_capacity(rat); i++) {
		rat->status[i] = REG_FREE;
		rat->note[i] = "";

		// 0-initialized
		rat->occupant[i] = calloc(rat->ntemps, sizeof(bool));
	}

	rat_init(rat);

	return rat;
}

static void rat_init(struct RAT* rat) {

	switch (rat->arch) {
		case RAT_ARCH_AVR:
			rat_init_avr(rat);
			break;
		case RAT_ARCH_X86:
			rat_init_x86(rat);
			break;
		default:
			break;
	}
}

static void rat_reserve_reg(struct RAT* rat, uint32_t reg, char* note) {

	rat->status[reg] = REG_RESERVED;
	rat->note[reg] = note;
}

static void rat_init_x86(struct RAT* rat) {

	rat_reserve_reg(rat, rat_return_reg(rat), "rax reserved for return value");

	rat_reserve_reg(rat, rat_scratch_reg(rat), "reserved as scratch register");

	rat_reserve_reg(rat, rat_stack_ptr(rat), " reserved (stack pointer)");

	rat_reserve_reg(rat, rat_base_ptr(rat), " reserved (frame base pointer)");
}

static void rat_init_avr(struct RAT* rat) {

	rat_reserve_reg(rat, rat_return_reg(rat), "reserved for return value");
	rat_reserve_reg(rat, rat_return_reg(rat) + 1, "reserved for return value");
	rat_reserve_reg(rat, rat_scratch_reg(rat), "reserved as scratch register");

	//r26 through r31 are X,Y,Z
	//and are used as pointer registers,
	//and should not be available for allocation
	rat_reserve_reg(rat, 26, "XL");
	rat_reserve_reg(rat, 27, "XH");

	//Y is our base pointer for the stack frame
	rat_reserve_reg(rat, rat_base_ptr(rat), "YL, frame ptr");
	rat_reserve_reg(rat, rat_base_ptr(rat) + 1, "YH, frame ptr");

	rat_reserve_reg(rat, 30, "ZL");
	rat_reserve_reg(rat, 31, "ZH");
}

void rat_dtor(struct RAT* rat) {

	for (int i = 0; i < rat_capacity(rat); i++) {
		free(rat->occupant[i]);
	}
	free(rat);
}

const char* regnames_x86[] = {
    [SD_REG_RAX] = "rax",
    [SD_REG_RBX] = "rbx",
    [SD_REG_RCX] = "rcx",
    [SD_REG_RDX] = "rdx",
    [SD_REG_RDI] = "rdi",
    [SD_REG_RSI] = "rsi",

    [SD_REG_RSP] = "rsp",
    [SD_REG_RBP] = "rbp",

    [SD_REG_R8] = "r8",
    [SD_REG_R9] = "r9",
    [SD_REG_R10] = "r10",
    [SD_REG_R11] = "r11",
    [SD_REG_R12] = "r12",
    [SD_REG_R13] = "r13",
    [SD_REG_R14] = "r14",
    [SD_REG_R15] = "r15",
};

char* rat_regname_x86(size_t i) {

	return (char*)regnames_x86[i];
}

static void rat_print_regname_x86(struct RAT* rat, size_t i) {

	assert(i < rat_capacity(rat));
	printf("%s", rat_regname_x86(i));
}

static void rat_print_regname(struct RAT* rat, size_t i) {

	switch (rat->arch) {
		// on avr, r16 is our scratch register
		case RAT_ARCH_AVR:
			printf("r%02ld", i);
			break;
		case RAT_ARCH_X86:
			rat_print_regname_x86(rat, i);
		default: return;
	}
}

static void rat_print_reg(struct RAT* rat, size_t i) {

	rat_print_regname(rat, i);
	printf(": ");

	switch (rat->status[i]) {

		case REG_OCCUPIED:
			for (size_t t = 0; t < rat->ntemps; t++) {

				if (rat->occupant[i][t]) {
					printf("t%ld, ", t);
				}
			}
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

void rat_print(struct RAT* rat) {

	printf("Register Allocation Table:\n");
	for (size_t i = 0; i < rat_capacity(rat); i++) {

		rat_print_reg(rat, i);
	}
	printf("------------\n");
}

bool rat_has_register(struct RAT* rat, uint32_t tmp_index) {

	for (int i = 0; i < rat_capacity(rat); i++) {
		if (rat->status[i] != REG_OCCUPIED) {
			continue;
		}
		if (rat->occupant[i][tmp_index]) {
			return true;
		}
	}
	return false;
}

int rat_get_register(struct RAT* rat, uint32_t tmp_index) {

	for (int i = 0; i < rat_capacity(rat); i++) {
		if (rat->status[i] != REG_OCCUPIED) {
			continue;
		}
		if (rat->occupant[i][tmp_index]) {
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

	for (size_t i = 0; i < rat->ntemps; i++) {
		if (rat->occupant[reg][i]) {
			return i;
		}
	}

	fprintf(stderr, "rat: did not find occupant of register %d\n", reg);
	exit(1);

	return 0;
}

bool rat_occupies(struct RAT* rat, uint8_t reg, uint32_t tmp_index) {

	assert(tmp_index < rat->ntemps);
	return rat->occupant[reg][tmp_index];
}

void rat_occupy(struct RAT* rat, uint8_t reg, uint32_t tmp_index, bool wide) {

	//occupy the register
	rat->status[reg] = REG_OCCUPIED;
	rat->occupant[reg][tmp_index] = true;

	if (wide) {
		rat->status[reg + 1] = REG_OCCUPIED;
		rat->occupant[reg + 1][tmp_index] = true;
	}
}

int32_t rat_find_reg_no_overlap(struct RAT* rat, bool* temps_conflict, size_t ntemps) {

	for (int reg = 0; reg < rat_capacity(rat); reg++) {
		if (rat->status[reg] == REG_RESERVED) {
			continue;
		}
		if (rat->status[reg] == REG_FREE) {
			return reg;
		}

		// reg is REG_OCCUPIED

		bool conflict = false;
		for (size_t t = 0; t < ntemps; t++) {
			const bool occupies = rat->occupant[reg][t];
			if (temps_conflict[t] && occupies) {
				conflict = true;
				break;
				;
			}
		}

		if (!conflict) {
			return reg;
		}
	}

	return -1;
}

uint32_t rat_ensure_register(struct RAT* rat, uint32_t tmp_index, bool high_regs_only, bool wide) {
	if (rat->arch == RAT_ARCH_X86) {
		wide = false;
	}
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

	return reg + 1 < rat_capacity(rat) && rat->status[reg + 1] == REG_OCCUPIED && (rat_occupant(rat, reg + 1) == tmp_index);
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

	if (rat->arch == RAT_ARCH_X86) {
		// on x86 there is no high/low regs therefore this parameter is irrelevant
		high_regs_only = false;
		wide = false;
	}

	//wide means we need a register pair.
	//we return the lower of the 2 registers

	//high_regs_only tells us at which register to start looking,
	//as there are some instructions such as 'ldi' which can only use a high register
	// (>= r16)

	const int start_inclusive = (high_regs_only) ? 16 : 0;

	for (int i = start_inclusive; i < rat_capacity(rat); i++) {

		if (rat->status[i] == REG_FREE) {

			if (wide) {
				if (i + 1 < rat_capacity(rat) && rat->status[i + 1] == REG_FREE) {
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

enum SD_REGISTER rat_scratch_reg(struct RAT* rat) {

	switch (rat->arch) {
		// on avr, r16 is our scratch register
		case RAT_ARCH_AVR: return 16;
		case RAT_ARCH_X86: return SD_REG_RBX;
	}
}

enum SD_REGISTER rat_return_reg(struct RAT* rat) {

	switch (rat->arch) {
		case RAT_ARCH_AVR:
			return 0;
		case RAT_ARCH_X86:
			return SD_REG_RAX;
	}
}

enum SD_REGISTER rat_base_ptr(struct RAT* rat) {

	switch (rat->arch) {
		case RAT_ARCH_AVR:
			return 28; // YL:YH
		case RAT_ARCH_X86:
			return SD_REG_RBP;
	}
}

enum SD_REGISTER rat_stack_ptr(struct RAT* rat) {

	switch (rat->arch) {
		case RAT_ARCH_AVR:
			printf("%s: not applicable for AVR\n", __func__);
			exit(1);
		case RAT_ARCH_X86:
			return SD_REG_RSP;
	}
}

uint16_t rat_capacity(struct RAT* rat) {

	switch (rat->arch) {
		case RAT_ARCH_AVR: return 32;
		case RAT_ARCH_X86:
			return sizeof(regnames_x86) / sizeof(regnames_x86[0]);
		default: return 0;
	}
}
