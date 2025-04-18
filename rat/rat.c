#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat.h"
#include "rat_x86.h"
#include "_struct.h"

// gives index of a free register
// the register still has to be occupied
// @returns < 0 on error
static int rat_get_free_register(struct RAT* rat, bool high_regs_only, bool wide);

static void rat_init(struct RAT* rat);

// @param arch    the target architecture
// @param ntemps  maximum number of temporaries
//                to make space for
struct RAT* rat_ctor(enum RAT_ARCH arch, size_t ntemps) {

	struct RAT* rat = malloc(sizeof(struct RAT));

	if (!rat) {
		return NULL;
	}

	rat->arch = arch;
	rat->ntemps = ntemps;

	for (int i = 0; i < rat_capacity(rat); i++) {
		rat->status[i] = REG_INVALID_ARCH;
		rat->note[i] = "";

		// 0-initialized
		rat->occupant[i] = calloc(rat->ntemps, sizeof(bool));
	}

	rat_init(rat);

	return rat;
}

void rat_dtor(struct RAT* rat) {
	for (int i = 0; i < rat_capacity(rat); i++) {
		free(rat->occupant[i]);
	}
	free(rat);
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

void rat_reserve_reg(struct RAT* rat, uint32_t reg, char* note) {

	rat->status[reg] = REG_RESERVED;
	rat->note[reg] = note;
}

static void rat_dump_regname(struct RAT* rat, FILE* fout, size_t i) {

	switch (rat->arch) {
		// on avr, r16 is our scratch register
		case RAT_ARCH_AVR:
			fprintf(fout, "r%02ld", i);
			break;
		case RAT_ARCH_X86:
			rat_print_regname_x86(rat, fout, i);
		default: return;
	}
}

static bool rat_dump_reg(struct RAT* rat, FILE* fout, size_t i) {

	const int status = rat->status[i];
	const bool valid = status != REG_INVALID_ARCH;

	if (!valid) {
		return true;
	}

	rat_dump_regname(rat, fout, i);

	fprintf(fout, ": ");

	switch (rat->status[i]) {

		case REG_OCCUPIED:
			for (size_t t = 0; t < rat->ntemps; t++) {

				if (rat->occupant[i][t]) {
					fprintf(fout, "t%ld, ", t);
				}
			}
			break;
		case REG_FREE:
			fprintf(fout, "%20s", " - ");
			break;
		case REG_RESERVED:
			fprintf(fout, "%20s", rat->note[i]);
			break;
		case REG_INVALID_ARCH:
			break;
	}

	fprintf(fout, "\n");

	return true;
}

bool rat_print(struct RAT* rat, FILE* fout) {

	if (fout == NULL) {
		return false;
	}

	fprintf(fout, "Register Allocation Table:\n");
	for (size_t i = 0; i < rat_capacity(rat); i++) {

		rat_dump_reg(rat, fout, i);
	}
	fprintf(fout, "------------\n");

	return true;
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

	fprintf(stderr, "[RAT] rat_get_register: t%d has no register\n", tmp_index);
	rat_print(rat, stdout);

	return -1;
}

int32_t rat_occupant(struct RAT* rat, uint8_t reg) {

	if (rat->status[reg] != REG_OCCUPIED) {
		fprintf(stderr, "[RAT] rat_occupant: requesting occupant for unoccupied register\n");
		rat_print(rat, stdout);
		return -1;
	}

	for (size_t i = 0; i < rat->ntemps; i++) {
		if (rat->occupant[reg][i]) {
			return i;
		}
	}

	fprintf(stderr, "rat: did not find occupant of register %d\n", reg);
	return -1;
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
		if (rat->status[reg] == REG_INVALID_ARCH) {
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

int32_t rat_ensure_register(struct RAT* rat, uint32_t tmp_index, bool high_regs_only, bool wide) {
	if (rat->arch == RAT_ARCH_X86) {
		wide = false;
	}
	//wide means we need a register pair, because of a 16 bit value

	//printf("rat ensure t%d, higher=%d",tmp_index, high_regs_only);
	//fflush(stdout);

	if (!rat_has_register(rat, tmp_index)) {

		int reg = rat_get_free_register(rat, high_regs_only, wide);

		if (reg < 0) {
			return -1;
		}

		rat_occupy(rat, reg, tmp_index, wide);
	}

	return rat_get_register(rat, tmp_index);
}

bool rat_is_wide(struct RAT* rat, uint32_t tmp_index) {

	//if the temporary occupies a register pair, return true

	int reg = rat_get_register(rat, tmp_index);

	return reg + 1 < rat_capacity(rat) && rat->status[reg + 1] == REG_OCCUPIED && (rat_occupant(rat, reg + 1) == tmp_index);
}

bool rat_free(struct RAT* rat, uint8_t reg) {

	switch (rat->status[reg]) {

		case REG_OCCUPIED:
			rat->status[reg] = REG_FREE;
			break;

		case REG_FREE:
			fprintf(stderr, "[RAT] double free.\n");
			return false;

		case REG_RESERVED:
			fprintf(stderr, "[RAT] trying to free reserved Register.\n");
			return false;

		case REG_INVALID_ARCH:
			break;

		default:
			fprintf(stderr, "unhandled case in %s\n", __func__);
			return false;
	}
	return true;
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

	fprintf(stderr, "RAT could not find a free register, they are all occupied.\n");
	rat_print(rat, stdout);
	return -1;
}

enum SD_REGISTER rat_scratch_reg(struct RAT* rat) {

	switch (rat->arch) {
		// on avr, r16 is our scratch register
		case RAT_ARCH_AVR: return r16;
		case RAT_ARCH_X86: return rat_scratch_reg_x86();
	}
}

enum SD_REGISTER rat_return_reg(struct RAT* rat) {

	switch (rat->arch) {
		case RAT_ARCH_AVR:
			return r0;
		case RAT_ARCH_X86:
			return rat_return_reg_x86();
	}
}

enum SD_REGISTER rat_base_ptr(struct RAT* rat) {

	switch (rat->arch) {
		case RAT_ARCH_AVR:
			return r28; // YL:YH
		case RAT_ARCH_X86:
			return rat_base_ptr_x86();
	}
}

enum SD_REGISTER rat_stack_ptr(struct RAT* rat) {

	switch (rat->arch) {
		case RAT_ARCH_AVR:
			fprintf(stderr, "%s: not applicable for AVR\n", __func__);
			assert(false);
		case RAT_ARCH_X86:
			return rat_stack_ptr_x86();
	}
}

uint16_t rat_capacity(struct RAT* rat) {

	return SD_REGISTER_END;
}

static bool is_callee_saved[] = {

    [SD_REG_RAX] = false, // return value
    [SD_REG_RBX] = true,
    [SD_REG_RCX] = false, // param
    [SD_REG_RDX] = false, // param
    [SD_REG_RDI] = false, // param
    [SD_REG_RSI] = false, // param

    [SD_REG_RSP] = false, // managed otherwise
    [SD_REG_RBP] = false, // managed otherwise

    [SD_REG_R8] = false, // param
    [SD_REG_R9] = false, // param
    [SD_REG_R10] = true,
    [SD_REG_R11] = true,
    [SD_REG_R12] = true,
    [SD_REG_R13] = true,
    [SD_REG_R14] = true,
    [SD_REG_R15] = true,
};

static bool rat_is_callee_saved(enum SD_REGISTER reg) {
	return is_callee_saved[reg];
}

bool rat_callee_must_save(struct RAT* rat, enum SD_REGISTER reg) {

	if (reg == SD_REG_RBP) {
		return true;
	}

	if (rat->status[reg] == REG_OCCUPIED) {
		return rat_is_callee_saved(reg);
	}

	return false;
}
