#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "ibuffer_shared.h"

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
#define low(X) (((X) & 0xff))
#define high(X) (((X) >> 8))

//some macros to make it easier to write
//in the codegen functions

#define avr_adc(dest, src, c) ibu2(AVR_ADC, dest, src, c)
#define avr_add(dest, src, c) ibu2(AVR_ADD, dest, src, c)
#define avr_adiw(dest, src, c) ibu3(AVR_ADIW, dest, src, c, "")
#define avr_and(dest, src, c) ibu2(AVR_AND, dest, src, c)
#define avr_andi(dest, k, c) ibu2(AVR_ANDI, dest, k, c)
#define avr_asr(dest, c) ibu1(AVR_ASR, dest, c)
#define avr_bclr(c) ibu1(AVR_BCLR, c, "")
#define avr_bld(dest, c) ibu2(AVR_BLD, dest, c, "")
//TODO: BRBC
//TODO: BRBS
//TODO: BRCC
//TODO: BRCS
//TODO: BREAK
#define avr_breq(label, c) ibu_branch(AVR_BREQ, label, c)
#define avr_brge(label, c) ibu_branch(AVR_BRGE, label, c)
//BRHC
//BRHS
//BRID
//BRIE
#define avr_brlo(label, c) ibu_branch(AVR_BRLO, label, c)
#define avr_brlt(label, c) ibu_branch(AVR_BRLT, label, c)
//BRMI
#define avr_brne(label, c) ibu_branch(AVR_BRNE, label, c)
#define avr_brpl(label, c) ibu_branch(AVR_BRPL, label, c)
//BRSH
//BRTC
//BRTS
//BRVC
//BRVS
//BSET
//BST
#define avr_call(label, c) ibu_branch(AVR_CALL, label, c)
//CBI
//CBR
//CLC
//CLH
//CLI
//CLN
#define avr_clr(dest, c) ibu1(AVR_CLR, dest, c)
//CLS
//CLT
//CLV
//CLZ
#define avr_com(dest, c) ibu1(AVR_COM, dest, c)
#define avr_cp(reg1, reg2, c) ibu2(AVR_CP, reg1, reg2, c)
#define avr_cpc(reg1, reg2, c) ibu2(AVR_CPC, reg1, reg2, c)
#define avr_cpi(reg1, k, c) ibu2(AVR_CPI, reg1, k, c)
#define avr_cpse(reg1, reg2, c) ibu2(AVR_CPSE, reg1, reg2, c)
#define avr_dec(dest, c) ibu1(AVR_DEC, dest, c)
#define avr_des(c) ibu1(AVR_DES, c, "")
//EICALL
//EIJMP
//ELPM
#define avr_eor(dest, src, c) ibu2(AVR_EOR, dest, src, c)
//FMUL
//FMULS
//FMULSU
//ICALL
//IJMP
#define avr_in(dest, addr, c) ibu2(AVR_IN, dest, addr, c)
#define avr_inc(dest, c) ibu1(AVR_INC, dest, c)
#define avr_jmp(label) ibu_branch(AVR_JMP, label, "")
//LAC
//LAS
//LAT
#define avr_ldX(dest, c) ibu1(AVR_LDX, dest, c)
#define avr_ldXpostInc(dest, c) ibu1(AVR_LDXpostInc, dest, c)
#define avr_ldY(dest, c) ibu1(AVR_LDY, dest, c)
#define avr_lddY(dest, k, c) ibu2(AVR_LDDY, dest, k, c)
//LDZ
#define avr_ldi(dest, k, c) ibu2(AVR_LDI, dest, k, c)
#define avr_lds(dest, addr, c) ibu2(AVR_LDS, dest, addr, c)
//LPM
#define avr_lsl(dest, c) ibu1(AVR_LSL, dest, c)
#define avr_lsr(dest, c) ibu1(AVR_LSR, dest, c)
#define avr_mov(dest, src, c) ibu2(AVR_MOV, dest, src, c)
//MOVW
#define avr_mul(reg1, reg2, c) ibu2(AVR_MUL, reg1, reg2, c)
//MULS
//MULSU
#define avr_neg(dest, c) ibu1(AVR_NEG, dest, c)
#define avr_nop(c) ibu0(AVR_NOP, c)
#define avr_or(dest, src, c) ibu2(AVR_OR, dest, src, c)
#define avr_ori(dest, k, c) ibu2(AVR_ORI, dest, k, c)
#define avr_out(addr, reg, c) ibu2(AVR_OUT, addr, reg, c)
#define avr_pop(dest, c) ibu1(AVR_POP, dest, c)
#define avr_push(dest, c) ibu1(AVR_PUSH, dest, c)
//RCALL
#define avr_ret(c) ibu0(AVR_RET, c)
//RETI
#define avr_rjmp(label, c) ibu_branch(AVR_RJMP, label, c)
#define avr_rol(dest, c) ibu1(AVR_ROL, dest, c)
#define avr_ror(dest, c) ibu1(AVR_ROR, dest, c)
#define avr_sbc(dest, src, c) ibu2(AVR_SBC, dest, src, c)
#define avr_sbci(dest, k, c) ibu2(AVR_SBCI, dest, k, c)
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
#define avr_stX(reg, c) ibu1(AVR_STX, reg, c)
#define avr_stXplus(reg, c) ibu1(AVR_STXPLUS, reg, c)
#define avr_stY(reg, c) ibu1(AVR_STY, reg, c)
#define avr_stdY(k, reg, c) ibu2(AVR_STDY, k, reg, c)
//stZ
#define avr_sts(addr, reg, c) ibu2(AVR_STS, addr, reg, c)
#define avr_sub(dest, src, c) ibu2(AVR_SUB, dest, src, c)
#define avr_subi(dest, k, c) ibu2(AVR_SUBI, dest, k, c)
//SWAP
#define avr_tst(dest, c) ibu1(AVR_TST, dest, c)
//WDR
//XCH

//not really an instr
#define avr_label(name) ibu4(LABEL, 0, 0, 0, name, "")
#define avra_def(str) ibu4(AVRA_DEF, 0, 0, 0, str, "")
#define avra_equ(str) ibu4(AVRA_EQU, 0, 0, 0, str, "")
