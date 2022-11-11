#ifndef SMALLDRAGON_TOPLEVEL_RAT_H
#define SMALLDRAGON_TOPLEVEL_RAT_H

#include <stdbool.h>
#include <inttypes.h>

#define RAT_CAPACITY (32)

//r16 is our scratch register
#define RAT_SCRATCH_REG (16)

struct RAT;

struct RAT* rat_ctor();

void rat_dtor(struct RAT* rat);

void rat_print(struct RAT* rat);

int rat_get_register(struct RAT* rat, uint32_t tmp_index);

uint32_t rat_occupant(struct RAT* rat, uint8_t reg);

uint32_t rat_ensure_register(struct RAT* rat, uint32_t tmp_index, bool high_regs_only);

void rat_free(struct RAT* rat, uint8_t reg);

#endif
