#ifndef SMALLDRAGON_TOPLEVEL_RAT_H
#define SMALLDRAGON_TOPLEVEL_RAT_H

#include <stdbool.h>
#include <inttypes.h>
#include <stddef.h>

enum RAT_ARCH {
	RAT_ARCH_AVR,
	RAT_ARCH_X86,
};

enum SD_REGISTER {
	SD_REG_RAX = 0,
	SD_REG_RBX,
	SD_REG_RCX,
	SD_REG_RDX,
	SD_REG_RDI,
	SD_REG_RSI,

	SD_REG_RSP,
	SD_REG_RBP,

	SD_REG_R8,
	SD_REG_R9,
	SD_REG_R10,
	SD_REG_R11,
	SD_REG_R12,
	SD_REG_R13,
	SD_REG_R14,
	SD_REG_R15,
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
enum SD_REGISTER rat_base_ptr(struct RAT* rat);
enum SD_REGISTER rat_stack_ptr(struct RAT* rat);

uint16_t rat_capacity(struct RAT* rat);

// x86 specific
char* rat_regname_x86(size_t i);

#endif
