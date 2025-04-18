#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

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

static enum SD_REGISTER param_regs[] = {
    SD_REG_RDI,
    SD_REG_RSI,
    SD_REG_RDX,
    SD_REG_RCX,
    SD_REG_R8,
    SD_REG_R9,
};

// https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
// https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#x86_64-64_bit
static enum SD_REGISTER param_regs_syscall[] = {
    SD_REG_RDI,
    SD_REG_RSI,
    SD_REG_RDX,
    SD_REG_R10,
    SD_REG_R8,
    SD_REG_R9,
};

enum SD_REGISTER rat_param_reg_x86(uint32_t index, bool is_syscall) {

	if (index >= 6) {
		fprintf(stderr, "%s: param count limited to 6\n", __func__);
	}
	assert(index < 6);

	if (is_syscall) {
		return param_regs_syscall[index];
	}

	return param_regs[index];
}

void rat_init_x86(struct RAT* rat) {

	for (uint32_t reg = SD_REG_START_X86 + 1; reg < SD_REG_END_X86; reg++) {
		rat->status[reg] = REG_FREE;
	}

	rat_reserve_reg(rat, rat_return_reg(rat), "rax reserved for return value");

	rat_reserve_reg(rat, rat_scratch_reg(rat), "reserved as scratch register");

	rat_reserve_reg(rat, rat_stack_ptr(rat), " reserved (stack pointer)");

	rat_reserve_reg(rat, rat_base_ptr(rat), " reserved (frame base pointer)");

	// reserve registers used for arguments
	// in our calling convention
	rat_reserve_reg(rat, SD_REG_RDI, "reserved (arg0)");
	rat_reserve_reg(rat, SD_REG_RSI, "reserved (arg1)");
	rat_reserve_reg(rat, SD_REG_RDX, "reserved (arg2)");
	rat_reserve_reg(rat, SD_REG_RCX, "reserved (arg3)");

	rat_reserve_reg(rat, SD_REG_R10, "reserved (arg4, syscall)");
	rat_reserve_reg(rat, SD_REG_R8, "reserved (arg4), (arg5, syscall)");
	rat_reserve_reg(rat, SD_REG_R9, "reserved (arg5), (arg6, syscall)");
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

const char* regnames_x86_4bytes[] = {
    [SD_REG_RAX] = "eax",
    [SD_REG_RBX] = "ebx",
    [SD_REG_RCX] = "ecx",
    [SD_REG_RDX] = "edx",
    [SD_REG_RDI] = "edi",
    [SD_REG_RSI] = "esi",

    [SD_REG_RSP] = "esp",
    [SD_REG_RBP] = "ebp",

    [SD_REG_R8] = "r8d",
    [SD_REG_R9] = "r9d",
    [SD_REG_R10] = "r10d",
    [SD_REG_R11] = "r11d",
    [SD_REG_R12] = "r12d",
    [SD_REG_R13] = "r13d",
    [SD_REG_R14] = "r14d",
    [SD_REG_R15] = "r15d",
};

const char* regnames_x86_2bytes[] = {
    [SD_REG_RAX] = "ax",
    [SD_REG_RBX] = "bx",
    [SD_REG_RCX] = "cx",
    [SD_REG_RDX] = "dx",
    [SD_REG_RDI] = "di",
    [SD_REG_RSI] = "si",

    [SD_REG_RSP] = "sp",
    [SD_REG_RBP] = "bp",

    [SD_REG_R8] = "r8w",
    [SD_REG_R9] = "r9w",
    [SD_REG_R10] = "r10w",
    [SD_REG_R11] = "r11w",
    [SD_REG_R12] = "r12w",
    [SD_REG_R13] = "r13w",
    [SD_REG_R14] = "r14w",
    [SD_REG_R15] = "r15w",
};

const char* regnames_x86_1bytes[] = {
    [SD_REG_RAX] = "al",
    [SD_REG_RBX] = "bl",
    [SD_REG_RCX] = "cl",
    [SD_REG_RDX] = "dl",
    [SD_REG_RDI] = "dil",
    [SD_REG_RSI] = "sil",

    [SD_REG_RSP] = "spl",
    [SD_REG_RBP] = "bpl",

    [SD_REG_R8] = "r8b",
    [SD_REG_R9] = "r9b",
    [SD_REG_R10] = "r10b",
    [SD_REG_R11] = "r11b",
    [SD_REG_R12] = "r12b",
    [SD_REG_R13] = "r13b",
    [SD_REG_R14] = "r14b",
    [SD_REG_R15] = "r15b",
};

char* rat_regname_x86(size_t i) {

	if (i < SD_REG_START_X86 || i >= SD_REG_END_X86) {
		fprintf(stderr, "%s: error: arg: %ld\n", __func__, i);
	}

	assert(i > SD_REG_START_X86);
	assert(i < SD_REG_END_X86);
	return (char*)regnames_x86[i];
}

char* rat_regname_x86_width(size_t i, uint8_t nbytes) {

	assert(i > SD_REG_START_X86);
	assert(i < SD_REG_END_X86);
	assert(nbytes <= 8);
	switch (nbytes) {
		case 8:
			return (char*)regnames_x86[i];
		case 4:
			return (char*)regnames_x86_4bytes[i];
		case 2:
			return (char*)regnames_x86_2bytes[i];
		case 1:
			return (char*)regnames_x86_1bytes[i];
	}

	fprintf(stderr, "error in %s\n", __func__);
	return NULL;
}

void rat_print_regname_x86(struct RAT* rat, FILE* fout, size_t i) {

	assert(i > SD_REG_START_X86);
	assert(i < SD_REG_END_X86);
	fprintf(fout, "%s", rat_regname_x86(i));
}
