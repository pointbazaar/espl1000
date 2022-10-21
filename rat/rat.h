#ifndef SMALLDRAGON_TOPLEVEL_RAT_H
#define SMALLDRAGON_TOPLEVEL_RAT_H

#include "../tables/symtable/symtable.h"

#include <stdbool.h>
#include <inttypes.h>

#define RAT_CAPACITY (32)

struct RAT;

struct RAT* rat_ctor(struct ST* st);
void rat_dtor(struct RAT* rat);

void rat_print(struct RAT* rat);

bool rat_has_register(struct RAT* rat, char* name);
int rat_get_register(struct RAT* rat, char* name);

//gives index of a free register, exits on failure.
//the register still has to be occupied
int rat_get_free_register(struct RAT* rat, bool high_regs_only);

//occupies that register 
void rat_occupy_register(struct RAT* rat, uint8_t reg, char* name);

bool rat_occupied(struct RAT* rat, uint8_t reg);

char* rat_occupant(struct RAT* rat, uint8_t reg);

#endif
