#ifndef IBUFFER_H
#define IBUFFER_H

#include <inttypes.h>

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
#define low(X)	(X & 0xff)
#define high(X)	(X >> 8)

struct Instr;

struct IBuffer;

struct IBuffer* ibu_ctor();

void ibu_dtor(struct IBuffer* ibu);

void ibu_write(struct IBuffer* ibu, FILE* fout);

void ibu_push(struct IBuffer* ibu, struct Instr* i);

void ibu_push4(struct IBuffer* ibu, enum IKEY key, int32_t x1, int32_t x2, int32_t x3, char* label, char* comment);

//constructions macros
#define ibu_branch(instr, label, comment)	ibu_push4(ibu, instr, 0,0,0, label, comment)

#define ibu0(instr, comment)					ibu_push4(ibu, instr,  0,  0,  0, "", comment)
#define ibu1(instr, x1, comment)				ibu_push4(ibu, instr, x1,  0,  0, "", comment)
#define ibu2(instr, x1, x2, comment)			ibu_push4(ibu, instr, x1, x2,  0, "", comment)
#define ibu3(instr, x1, x2, x3, comment)		ibu_push4(ibu, instr, x1, x2, x3, "", comment)
#define ibu4(instr, x1, x2, x3, s, comment)		ibu_push4(ibu, instr, x1, x2, x3,  s, comment)

//some macros to make it easier to write
//in the codegen functions

#define adc(dest, src, c) 		ibu2(ADC, dest, src, c)
#define add(dest, src, c) 		ibu2(ADD, dest, src, c)
#define adiw(dest, src, c) 	ibu3(ADIW, dest, src, c, "")
#define and(dest, src, c) 		ibu2(AND, dest, src, c)
#define andi(dest, k, c) 		ibu2(ANDI, dest, k, c)
#define asr(dest, c)			ibu1(ASR, dest, c)
#define bclr(c)				ibu1(BCLR, c, "")
#define bld(dest, c)		ibu2(BLD, dest, c, "")
//TODO: BRBC
//TODO: BRBS
//TODO: BRCC
//TODO: BRCS
//TODO: BREAK
#define breq(label, c)			ibu_branch(BREQ, label, c)
#define brge(label, c)			ibu_branch(BRGE, label, c)
//BRHC
//BRHS
//BRID
//BRIE
#define brlo(label, c)			ibu_branch(BRLO, label, c)
#define brlt(label, c)			ibu_branch(BRLT, label, c)
//BRMI
#define brne(label, c)			ibu_branch(BRNE, label, c)
//BRPL
//BRSH
//BRTC
//BRTS
//BRVC
//BRVS
//BSET
//BST
#define call(label, c)			ibu_branch(CALL, label, c)
//CBI
//CBR
//CLC
//CLH
//CLI
//CLN
#define clr(dest, c)		ibu1(CLR, dest, c)
//CLS
//CLT
//CLV
//CLZ
#define com(dest, c)			ibu1(COM, dest, c)
#define cp(reg1, reg2, c)		ibu2(CP, reg1, reg2, c)
#define cpc(reg1, reg2, c)		ibu2(CPC, reg1, reg2, c)
#define cpi(reg1, k, c)		ibu2(CPI, reg1, k, c)
#define cpse(reg1, reg2, c)	ibu2(CPSE, reg1, reg2, c)
#define dec(dest, c)			ibu1(DEC, dest, c)
#define des(c)				ibu1(DES, c, "")
//EICALL
//EIJMP
//ELPM
#define eor(dest, src, c)		ibu2(EOR, dest, src, c)
//FMUL
//FMULS
//FMULSU
//ICALL
//IJMP
#define in(dest, addr, c)	ibu2(IN, dest, addr, c)
#define inc(dest, c)			ibu1(INC, dest, c)
#define jmp(label)			ibu_branch(JMP, label, "")
//LAC
//LAS
//LAT
#define ldX(dest, c)		ibu1(LDX, dest, c)
#define ldXpostInc(dest, c)	ibu1(LDXpostInc, dest, c)
#define ldY(dest, c)		ibu1(LDY, dest, c)
#define lddY(dest, k, c)	ibu2(LDDY, dest, k, c)
//LDZ
#define ldi(dest, k, c)		ibu2(LDI, dest, k, c)
#define lds(dest, addr, c)		ibu2(LDS, dest, addr, c)
//LPM
#define lsl(dest, c)			ibu1(LSL, dest, c)
#define lsr(dest, c)			ibu1(LSR, dest, c)
#define mov(dest, src, c)		ibu2(MOV, dest, src, c)
//MOVW
#define mul(reg1, reg2, c)		ibu2(MUL, reg1, reg2, c)
//MULS
//MULSU
#define neg(dest, c)			ibu1(NEG, dest, c)
#define nop(c)				ibu0(NOP, c)
#define or(dest, src, c)		ibu2(OR, dest, src, c)
#define ori(dest, k, c)		ibu2(ORI, dest, k, c)
#define out(addr, reg, c)	ibu2(OUT, addr, reg, c)
#define pop(dest, c)	ibu1(POP, dest, c)
#define push(dest, c)	ibu1(PUSH, dest, c)
//RCALL
#define ret(c)				ibu0(RET, c)
//RETI
#define rjmp(label, c)			ibu_branch(RJMP, label, c)
#define rol(dest, c)			ibu1(ROL, dest, c)
#define ror(dest, c)			ibu1(ROR, dest, c)
#define sbc(dest, src, c)		ibu2(SBC, dest, src, c)
#define sbci(dest, k, c)		ibu2(SBCI, dest, k, c)
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
#define stX(reg, c)			ibu1(STX, reg, c)
#define stXplus(reg, c)			ibu1(STXPLUS, reg, c)
#define stY(reg, c)			ibu1(STY, reg, c)
#define stdY(k, reg, c)		ibu2(STDY, k, reg, c)
//stZ
#define sts(addr, reg, c)		ibu2(STS, addr, reg, c)
#define sub(dest, src, c)		ibu2(SUB, dest, src, c)
#define subi(dest, k, c)		ibu2(SUBI, dest, k, c)
//SWAP
#define tst(dest, c)			ibu1(TST, dest, c)
//WDR
//XCH


//not really an instr
#define label(name)			ibu4(LABEL, 0,0,0, name, "")

#endif
