#pragma once

#include <inttypes.h>
#include <stdio.h>

#include "ikey.h"

struct Instr;

struct IBuffer;

struct IBuffer* ibu_ctor();

void ibu_dtor(struct IBuffer* ibu);

void ibu_write(struct IBuffer* ibu, FILE* fout);

void ibu_push(struct IBuffer* ibu, struct Instr* i);

void ibu_push4(struct IBuffer* ibu, enum IKEY key, int64_t x1, int64_t x2, int64_t x3, char* label, char* comment);

//constructions macros
#define ibu_branch(instr, label, comment) ibu_push4(ibu, instr, 0, 0, 0, label, comment)

#define ibu0(instr, comment) ibu_push4(ibu, instr, 0, 0, 0, "", comment)
#define ibu1(instr, x1, comment) ibu_push4(ibu, instr, x1, 0, 0, "", comment)
#define ibu2(instr, x1, x2, comment) ibu_push4(ibu, instr, x1, x2, 0, "", comment)
#define ibu3(instr, x1, x2, x3, comment) ibu_push4(ibu, instr, x1, x2, x3, "", comment)
#define ibu4(instr, x1, x2, x3, s, comment) ibu_push4(ibu, instr, x1, x2, x3, s, comment)

//some macros to make it easier to write
//in the codegen functions

#define mov_const(dest, src, c) ibu2(X86_MOV_CONST, dest, src, c)
#define mov_regs(dest, src, c) ibu2(X86_MOV_REGS, dest, src, c)
#define mov_load(dest, src, c) ibu2(X86_MOV_LOAD, dest, src, c)
#define mov_store(dest, src, c) ibu2(X86_MOV_STORE, dest, src, c)
#define mov_load_width(dest, src, width, c) ibu3(X86_MOV_LOAD_WIDTH, dest, src, width, c)
#define movzx_load_width(dest, src, width, c) ibu3(X86_MOVZX_LOAD_WIDTH, dest, src, width, c)
#define movzx_regs(dest, src, width, c) ibu3(X86_MOVZX_REGS_WIDTH, dest, src, width, c)
#define mov_store_width(dest, src, width, c) ibu3(X86_MOV_STORE_WIDTH, dest, src, width, c)

#define mov_const_symbol(dest, name, c) ibu_push4(ibu, X86_MOV_CONST_SYMBOL, dest, 0, 0, name, c)

#define cmove(dest, src, c) ibu2(X86_CMOVE, dest, src, c)
#define xchg(dest, src, c) ibu2(X86_XCHG, dest, src, c)

#define push(dest, c) ibu1(X86_PUSH, dest, c)
#define pusha(dest, c) ibu1(X86_PUSHA, dest, c)
#define pop(dest, c) ibu1(X86_POP, dest, c)
#define popa(dest, c) ibu1(X86_POPA, dest, c)

#define add(dest, src, c) ibu2(X86_ADD, dest, src, c)
#define sub(dest, src, c) ibu2(X86_SUB, dest, src, c)
#define mul(dest, src, c) ibu2(X86_IMUL, dest, src, c)
// 'dest' is the divisor.
// The dividend is in rdx:rax
#define div(dest, c) ibu1(X86_DIV, dest, c)

#define inc(dest, c) ibu1(X86_INC, dest, c)
#define dec(dest, c) ibu1(X86_DEC, dest, c)
#define neg(dest, c) ibu1(X86_NEG, dest, c)
#define cmp(dest, src, c) ibu2(X86_CMP, dest, src, c)
#define cmp_const(dest, value, c) ibu2(X86_CMP_CONST, dest, value, c)
#define test(dest, src, c) ibu2(X86_TEST, dest, src, c)

// set bit if equals
#define sete(dest, c) ibu1(X86_SETE, dest, c)

#define and(dest, src, c) ibu2(X86_AND, dest, src, c)
#define or(dest, src, c) ibu2(X86_OR, dest, src, c)
#define xor(dest, src, c) ibu2(X86_XOR, dest, src, c)
#define not(dest, c) ibu1(X86_NOT, dest, c)

#define shl(dest, imm, c) ibu2(X86_SHL, dest, imm, c)
#define shr(dest, imm, c) ibu2(X86_SHR, dest, imm, c)

#define jmp(label, c) ibu_branch(X86_JMP, label, "")
#define je(label, c) ibu_branch(X86_JE, label, "")
#define jne(label, c) ibu_branch(X86_JNE, label, "")
#define jg(label, c) ibu_branch(X86_JG, label, "")
#define jge(label, c) ibu_branch(X86_JGE, label, "")
#define jl(label, c) ibu_branch(X86_JL, label, "")
#define jle(label, c) ibu_branch(X86_JLE, label, "")
#define jz(label, c) ibu_branch(X86_JZ, label, c)

#define call(label, c) ibu_branch(X86_CALL, label, c)
#define icall(reg, c) ibu1(X86_ICALL, reg, c)
#define ret(c) ibu0(X86_RET, c)
#define nop(c) ibu0(X86_NOP, c)

#define x86_int(x, c) ibu1(X86_INT, x, c)
#define x86_syscall(c) ibu0(X86_SYSCALL, c)

//not really an instr
#define label(name) ibu4(LABEL, 0, 0, 0, name, "")
