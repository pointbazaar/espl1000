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

#define adc(dest, src) 		ibu2(ADC, dest, src, "")
#define add(dest, src) 		ibu2(ADD, dest, src, "")
#define adiw(dest, src, c) 	ibu3(ADIW, dest, src, c, "")
#define and(dest, src) 		ibu2(AND, dest, src, "")
#define andi(dest, c) 		ibu2(ANDI, dest, c, "")
#define asr(dest)			ibu1(ASR, dest, "")
#define bclr(c)				ibu1(BCLR, c, "")
#define bld(dest, c)		ibu2(BLD, dest, c, "")
//TODO: BRBC
//TODO: BRBS
//TODO: BRCC
//TODO: BRCS
//TODO: BREAK
#define breq(label)			ibu_branch(BREQ, label, "")
#define brge(label)			ibu_branch(BRGE, label, "")
//BRHC
//BRHS
//BRID
//BRIE
#define brlo(label)			ibu_branch(BRLO, label, "")
#define brlt(label)			ibu_branch(BRLT, label, "")
//BRMI
#define brne(label)			ibu_branch(BRNE, label, "")
//BRPL
//BRSH
//BRTC
//BRTS
//BRVC
//BRVS
//BSET
//BST
#define call(label)			ibu_branch(CALL, label, "")
//CBI
//CBR
//CLC
//CLH
//CLI
//CLN
#define clr(dest, comment)		ibu1(CLR, dest, comment)
//CLS
//CLT
//CLV
//CLZ
#define com(dest)			ibu1(COM, dest, "")
#define cp(reg1, reg2)		ibu2(CP, reg1, reg2, "")
#define cpc(reg1, reg2)		ibu2(CPC, reg1, reg2, "")
#define cpi(reg1, c)		ibu2(CPI, reg1, c, "")
#define cpse(reg1, reg2)	ibu2(CPSE, reg1, reg2, "")
#define dec(dest)			ibu1(DEC, dest, "")
#define des(c)				ibu1(DES, c, "")
//EICALL
//EIJMP
//ELPM
#define eor(dest, src)		ibu2(EOR, dest, src, "")
//FMUL
//FMULS
//FMULSU
//ICALL
//IJMP
#define in(dest, addr, comment)	ibu2(IN, dest, addr, comment)
#define inc(dest)			ibu1(INC, dest, "")
#define jmp(label)			ibu_branch(JMP, label, "")
//LAC
//LAS
//LAT
#define ldX(dest, comment)		ibu1(LDX, dest, comment)
#define ldY(dest, comment)		ibu1(LDY, dest, comment)
#define lddY(dest, c, comment)	ibu2(LDDY, dest, c, comment)
//LDZ
#define ldi(dest, c, comment)	ibu2(LDI, dest, c, comment)
#define lds(dest, addr)		ibu2(LDS, dest, addr, "")
//LPM
#define lsl(dest)			ibu1(LSL, dest, "")
#define lsr(dest)			ibu1(LSR, dest, "")
#define mov(dest, src)		ibu2(MOV, dest, src, "")
//MOVW
#define mul(reg1, reg2)		ibu2(MUL, reg1, reg2, "")
//MULS
//MULSU
#define neg(dest)			ibu1(NEG, dest, "")
#define nop()				ibu0(NOP, "")
#define or(dest, src)		ibu2(OR, dest, src, "")
#define ori(dest, c)		ibu2(ORI, dest, c, "")
#define out(addr, reg, comment)	ibu2(OUT, addr, reg, comment)
#define pop(dest, comment)	ibu1(POP, dest, comment)
#define push(dest, comment)	ibu1(PUSH, dest, comment)
//RCALL
#define ret()				ibu0(RET, "")
//RETI
#define rjmp(label)			ibu_branch(RJMP, label, "")
//ROL
//ROR
#define sbc(dest, src, comment)		ibu2(SBC, dest, src, comment)
#define sbci(dest, c, comment)		ibu2(SBCI, dest, c, comment)
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
#define stX(reg, comment)			ibu1(STX, reg, comment)
#define stY(reg, comment)			ibu1(STY, reg, comment)
#define stdY(c, reg, comment)		ibu2(STDY, c, reg, comment)
//stZ
#define sts(addr, reg)		ibu2(STS, addr, reg, "")
#define sub(dest, src)		ibu2(SUB, dest, src, "")
#define subi(dest, c)		ibu2(SUBI, dest, c, "")
//SWAP
#define tst(dest)			ibu1(TST, dest, "")
//WDR
//XCH


//not really an instr
#define label(name)			ibu4(LABEL, 0,0,0, name, "")

#endif
