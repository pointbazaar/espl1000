
#include "ikey.h"

#define def(X,Y) [X] = Y,

char* MNEM[] = {
	//order like in the instruction summary
	
	//arithmetic and logic instructions
	def(ADD, "add")
	def(ADC, "adc")
	def(ADIW, "adiw")
	def(SUB, "sub")
	def(SUBI, "subi")
	def(SBC, "sbc")
	def(SBCI, "sbci")
	def(SBIW, "sbiw")
	def(AND, "and")
	def(ANDI, "andi")
	def(OR, "or")
	def(ORI, "ori")
	def(EOR, "eor")
	def(COM, "com")
	def(NEG, "neg")
	def(SBR, "sbr")
	def(CBR, "cbr")
	def(INC, "inc")
	def(DEC, "dec")
	def(TST, "tst")
	def(CLR, "clr")
	def(SER, "ser")
	def(MUL, "mul")
	def(MULS, "muls")
	def(MULSU, "mulsu")
	def(FMUL, "fmul")
	def(FMULS, "fmuls")
	def(FMULSU, "fmulsu")
	def(DES, "des")
	
	//branch instructions
	def(RJMP, "rjmp")
	def(IJMP, "ijmp")
	def(EIJMP, "eijmp")
	def(JMP, "jmp")
	def(RCALL, "rcall")
	def(ICALL, "icall")
	def(EICALL, "eicall")
	def(CALL, "call")
	def(RET, "ret")
	def(RETI, "reti")
	def(CPSE, "cpse")
	def(CP, "cp")
	def(CPC, "cpc")
	def(CPI, "cpi")
	def(SBRC, "sbrc")
	def(SBRS, "sbrs")
	def(SBIC, "sbic")
	def(SBIS, "sbis")
	def(BRBS, "brbs")
	def(BRBC, "brbc")
	def(BREQ, "breq")
	def(BRNE, "brne")
	def(BRCS, "brcs")
	def(BRCC, "brcc")
	def(BRSH, "brsh")
	def(BRLO, "brlo")
	def(BRMI, "brmi")
	def(BRPL, "brpl")
	def(BRGE, "brge")
	def(BRLT, "brlt")
	def(BRHS, "brhs")
	def(BRHC, "brhc")
	def(BRTS, "brts")
	def(BRTC, "brtc")
	def(BRVS, "brvs")
	def(BRVC, "brvc")
	def(BRIE, "brie")
	def(BRID, "brid")
	
	//data transfer instructions
	def(MOV, "mov")
	def(MOVW, "movw")
	def(LDI, "ldi")
	
	def(LDS, "lds")
	
	//ld instructions
	def(LDX, "ld")
	def(LDY, "ld")
	def(LDZ, "ld")
	def(LDDY, "ldd")
	def(LDDZ, "ldd")
	
	def(STS, "sts")
	
	//st instructions
	def(STX, "st")
	def(STY, "st")
	def(STZ, "st")
	def(STDY, "std")
	
	def(LPM, "lpm")
	def(ELPM, "elpm")
	def(SPM, "spm")
	def(IN, "in")
	def(OUT, "out")
	def(PUSH, "push")
	def(POP, "pop")
	def(XCH, "xch")
	def(LAS, "las")
	def(LAC, "lac")
	def(LAT, "lat")
	
	//bit and bit-test instructions
	def(LSL, "lsl")
	def(LSR, "lsr")
	def(ROL, "rol")
	def(ROR, "ror")
	def(ASR, "asr")
	def(SWAP, "swap")
	def(SBI, "sbi")
	def(CBI, "cbi")
	
	def(BST, "bst")
	def(BLD, "bld")
	def(BSET, "bset")
	def(BCLR, "bclr")
	def(SEC, "sec")
	def(CLC, "clc")
	def(SEN, "sen")
	def(CLN, "cln")
	def(SEZ, "sez")
	def(CLZ, "clz")
	def(SEI, "sei")
	def(CLI, "cli")
	def(SES, "ses")
	def(CLS, "cls")
	def(SEV, "sev")
	def(CLV, "clv")
	def(SET, "set")
	def(CLT, "clt")
	def(SEH, "seh")
	def(CLH, "clh")
	
	//mcu control instructions
	def(BREAK, "break")
	def(NOP, "nop")
	def(SLEEP, "sleep")
	def(WDR, "wdr")
};
