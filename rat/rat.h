#ifndef SMALLDRAGON_TOPLEVEL_RAT_H
#define SMALLDRAGON_TOPLEVEL_RAT_H

#include <stdbool.h>
#include <inttypes.h>

enum RAT_ARCH {
	RAT_ARCH_AVR,
	RAT_ARCH_X86,
};

struct RAT;

struct RAT* rat_ctor(enum RAT_ARCH arch);

void rat_dtor(struct RAT* rat);

void rat_print(struct RAT* rat);

int rat_get_register(struct RAT* rat, uint32_t tmp_index);

uint32_t rat_occupant(struct RAT* rat, uint8_t reg);

uint32_t rat_ensure_register(struct RAT* rat, uint32_t tmp_index, bool high_regs_only, bool wide);

bool rat_is_wide(struct RAT* rat, uint32_t tmp_index);

void rat_free(struct RAT* rat, uint8_t reg);

uint16_t rat_scratch_reg(struct RAT* rat);

uint16_t rat_capacity(struct RAT* rat);

#endif
