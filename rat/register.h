#pragma once

#include <stdbool.h>
#include <inttypes.h>
#include <stddef.h>

struct RAT;

enum SD_REGISTER {

	// AVR
	r0 = 0,
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7,
	r8,
	r9,
	r10,
	r11,
	r12,
	r13,
	r14,
	r15,
	r16,
	r17,
	r18,
	r19,
	r20,
	r21,
	r22,
	r23,
	r24,
	r25,
	r26,
	r27,
	r28,
	r29,
	r30,
	r31,

	// x86-64
	SD_REG_START_X86,

	SD_REG_RAX,
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

	SD_REG_END_X86,

	SD_REGISTER_END,
};
