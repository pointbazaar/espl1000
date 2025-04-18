#pragma once

#include <stdbool.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#include "register.h"

enum RAT_ARCH {
	RAT_ARCH_AVR,
	RAT_ARCH_X86,
};

struct RAT;

// @returns NULL on error
struct RAT* rat_ctor(enum RAT_ARCH arch, size_t ntemps);

void rat_dtor(struct RAT* rat);

void rat_init_avr(struct RAT* rat);

// @returns false on error
bool rat_print(struct RAT* rat, FILE* fout);

void rat_reserve_reg(struct RAT* rat, uint32_t reg, char* note);

// @returns    true if 'tmp_index' occupies a register
bool rat_has_register(struct RAT* rat, uint32_t tmp_index);

// @returns    the register where 'tmp_index' resides
// @returs < 0 on error
int rat_get_register(struct RAT* rat, uint32_t tmp_index);

// @returns    the lowest temporary that occupies the register
// @returns < 0 on error
int32_t rat_occupant(struct RAT* rat, uint8_t reg);

// @param reg         the register
// @param temp_index  the index of the temporary to look for
// @returns           true if 'temp' is held by 'reg'
bool rat_occupies(struct RAT* rat, uint8_t reg, uint32_t tmp_index);

// occupies the register forcefully, without any checks
// @param reg         which register to occupy
// @param tmp_index   the IR temporary
void rat_occupy(struct RAT* rat, uint8_t reg, uint32_t tmp_index, bool wide);

// @param temps_conflict   'true' for all temporaries
//                         we want to avoid sharing
//                         a register with
// @param ntemps           size of 'temps_conflict'
// @returns        the register number found that
//                 does not contain the temporaries listed as
//                 overlap
// @returns        -1 if such a register could not be found
int32_t rat_find_reg_no_overlap(struct RAT* rat, bool* temps_conflict, size_t ntemps);

// @returns < 0 if no register was found
int32_t rat_ensure_register(struct RAT* rat, uint32_t tmp_index, bool high_regs_only, bool wide);

bool rat_is_wide(struct RAT* rat, uint32_t tmp_index);

// @returns false on error
bool rat_free(struct RAT* rat, uint8_t reg);

enum SD_REGISTER rat_scratch_reg(struct RAT* rat);
enum SD_REGISTER rat_return_reg(struct RAT* rat);
enum SD_REGISTER rat_base_ptr(struct RAT* rat);
enum SD_REGISTER rat_stack_ptr(struct RAT* rat);

uint16_t rat_capacity(struct RAT* rat);

// @returns     true if the callee needs to save this register
//              false if the callee does not touch this register
//              and thus does not need to save it
// @param rat   the RAT belonging to the callee
bool rat_callee_must_save(struct RAT* rat, enum SD_REGISTER reg);
