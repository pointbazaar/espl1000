#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "../util/exit_malloc/exit_malloc.h"
#include "rat_x86.h"
#include "rat.h"
#include "register.h"
#include "_struct.h"

enum SD_REGISTER rat_return_reg_x86() {
	return SD_REG_RAX;
}

enum SD_REGISTER rat_base_ptr_x86() {
	return SD_REG_RBP;
}

enum SD_REGISTER rat_stack_ptr_x86() {
	return SD_REG_RSP;
}

enum SD_REGISTER rat_scratch_reg_x86() {
	return SD_REG_RBX;
}

void rat_init_x86(struct RAT* rat) {

	for (uint32_t reg = SD_REG_START_X86 + 1; reg < SD_REG_END_X86; reg++) {
		rat->status[reg] = REG_FREE;
	}

	rat_reserve_reg(rat, rat_return_reg(rat), "rax reserved for return value");

	rat_reserve_reg(rat, rat_scratch_reg(rat), "reserved as scratch register");

	rat_reserve_reg(rat, rat_stack_ptr(rat), " reserved (stack pointer)");

	rat_reserve_reg(rat, rat_base_ptr(rat), " reserved (frame base pointer)");
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

	assert(i > SD_REG_START_X86);
	assert(i < SD_REG_END_X86);
	return (char*)regnames_x86[i];
}

void rat_print_regname_x86(struct RAT* rat, size_t i) {

	assert(i > SD_REG_START_X86);
	assert(i < SD_REG_END_X86);
	printf("%s", rat_regname_x86(i));
}
