#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include "ikey.h"

//AVR pointer registers
#define XL 26
#define XH 27
#define YL 28
#define YH 29
#define ZL 30
#define ZH 31

#define SPL 0x3d
#define SPH 0x3e

#define RAMEND 0x085f
#define low(X) (X & 0xff)
#define high(X) (X >> 8)

struct Instr;

struct IBuffer;

// @returns NULL on error
struct IBuffer* ibu_ctor();

void ibu_dtor(struct IBuffer* ibu);

void ibu_write(struct IBuffer* ibu, FILE* fout);

void ibu_push(struct IBuffer* ibu, struct Instr* i);

// @returns false on error
bool ibu_push4(struct IBuffer* ibu, enum IKEY key, int32_t x1, int32_t x2, int32_t x3, char* label, char* comment);

//constructions macros
#define ibu_branch(instr, label, comment) ibu_push4(ibu, instr, 0, 0, 0, label, comment)

#define ibu0(instr, comment) ibu_push4(ibu, instr, 0, 0, 0, "", comment)
#define ibu1(instr, x1, comment) ibu_push4(ibu, instr, x1, 0, 0, "", comment)
#define ibu2(instr, x1, x2, comment) ibu_push4(ibu, instr, x1, x2, 0, "", comment)
#define ibu3(instr, x1, x2, x3, comment) ibu_push4(ibu, instr, x1, x2, x3, "", comment)
#define ibu4(instr, x1, x2, x3, s, comment) ibu_push4(ibu, instr, x1, x2, x3, s, comment)

//some macros to make it easier to write
//in the codegen functions

#define adc(dest, src, c) ibu2(AVR_ADC, dest, src, c)
#define add(dest, src, c) ibu2(AVR_ADD, dest, src, c)
#define adiw(dest, src, c) ibu3(AVR_ADIW, dest, src, c, "")
#define and(dest, src, c) ibu2(AVR_AND, dest, src, c)
#define andi(dest, k, c) ibu2(AVR_ANDI, dest, k, c)
#define asr(dest, c) ibu1(AVR_ASR, dest, c)
#define bclr(c) ibu1(AVR_BCLR, c, "")
#define bld(dest, c) ibu2(AVR_BLD, dest, c, "")
//TODO: BRBC
//TODO: BRBS
//TODO: BRCC
//TODO: BRCS
//TODO: BREAK
#define breq(label, c) ibu_branch(AVR_BREQ, label, c)
#define brge(label, c) ibu_branch(AVR_BRGE, label, c)
//BRHC
//BRHS
//BRID
//BRIE
#define brlo(label, c) ibu_branch(AVR_BRLO, label, c)
#define brlt(label, c) ibu_branch(AVR_BRLT, label, c)
//BRMI
#define brne(label, c) ibu_branch(AVR_BRNE, label, c)
#define brpl(label, c) ibu_branch(AVR_BRPL, label, c)
//BRSH
//BRTC
//BRTS
//BRVC
//BRVS
//BSET
//BST
#define call(label, c) ibu_branch(AVR_CALL, label, c)
//CBI
//CBR
//CLC
//CLH
//CLI
//CLN
#define clr(dest, c) ibu1(AVR_CLR, dest, c)
//CLS
//CLT
//CLV
//CLZ
#define com(dest, c) ibu1(AVR_COM, dest, c)
#define cp(reg1, reg2, c) ibu2(AVR_CP, reg1, reg2, c)
#define cpc(reg1, reg2, c) ibu2(AVR_CPC, reg1, reg2, c)
#define cpi(reg1, k, c) ibu2(AVR_CPI, reg1, k, c)
#define cpse(reg1, reg2, c) ibu2(AVR_CPSE, reg1, reg2, c)
#define dec(dest, c) ibu1(AVR_DEC, dest, c)
#define des(c) ibu1(AVR_DES, c, "")
//EICALL
//EIJMP
//ELPM
#define eor(dest, src, c) ibu2(AVR_EOR, dest, src, c)
//FMUL
//FMULS
//FMULSU
//ICALL
//IJMP
#define in(dest, addr, c) ibu2(AVR_IN, dest, addr, c)
#define inc(dest, c) ibu1(AVR_INC, dest, c)
#define jmp(label) ibu_branch(AVR_JMP, label, "")
//LAC
//LAS
//LAT
#define ldX(dest, c) ibu1(AVR_LDX, dest, c)
#define ldXpostInc(dest, c) ibu1(AVR_LDXpostInc, dest, c)
#define ldY(dest, c) ibu1(AVR_LDY, dest, c)
#define lddY(dest, k, c) ibu2(AVR_LDDY, dest, k, c)
//LDZ
#define ldi(dest, k, c) ibu2(AVR_LDI, dest, k, c)
#define lds(dest, addr, c) ibu2(AVR_LDS, dest, addr, c)
//LPM
#define lsl(dest, c) ibu1(AVR_LSL, dest, c)
#define lsr(dest, c) ibu1(AVR_LSR, dest, c)
#define mov(dest, src, c) ibu2(AVR_MOV, dest, src, c)
//MOVW
#define mul(reg1, reg2, c) ibu2(AVR_MUL, reg1, reg2, c)
//MULS
//MULSU
#define neg(dest, c) ibu1(AVR_NEG, dest, c)
#define nop(c) ibu0(AVR_NOP, c)
#define or(dest, src, c) ibu2(AVR_OR, dest, src, c)
#define ori(dest, k, c) ibu2(AVR_ORI, dest, k, c)
#define out(addr, reg, c) ibu2(AVR_OUT, addr, reg, c)
#define pop(dest, c) ibu1(AVR_POP, dest, c)
#define push(dest, c) ibu1(AVR_PUSH, dest, c)
//RCALL
#define ret(c) ibu0(AVR_RET, c)
//RETI
#define rjmp(label, c) ibu_branch(AVR_RJMP, label, c)
#define rol(dest, c) ibu1(AVR_ROL, dest, c)
#define ror(dest, c) ibu1(AVR_ROR, dest, c)
#define sbc(dest, src, c) ibu2(AVR_SBC, dest, src, c)
#define sbci(dest, k, c) ibu2(AVR_SBCI, dest, k, c)
//SBI
//SBIC
//SBIS
//SBIW
//SBR
//SBRC
//SBRS
//SEC
//SEH
//SEI
//SEN
//SER
//SES
//SET
//SEV
//SEZ
//SLEEP
//SPM
#define stX(reg, c) ibu1(AVR_STX, reg, c)
#define stXplus(reg, c) ibu1(AVR_STXPLUS, reg, c)
#define stY(reg, c) ibu1(AVR_STY, reg, c)
#define stdY(k, reg, c) ibu2(AVR_STDY, k, reg, c)
//stZ
#define sts(addr, reg, c) ibu2(AVR_STS, addr, reg, c)
#define sub(dest, src, c) ibu2(AVR_SUB, dest, src, c)
#define subi(dest, k, c) ibu2(AVR_SUBI, dest, k, c)
//SWAP
#define tst(dest, c) ibu1(AVR_TST, dest, c)
//WDR
//XCH

//not really an instr
#define label(name) ibu4(LABEL, 0, 0, 0, name, "")
