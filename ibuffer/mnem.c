
#include "ikey.h"

#define def(X, Y) [X] = Y,

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
	def(LDXpostInc, "ld")
	    def(LDY, "ld")
		def(LDYpostInc, "ld")
		    def(LDZ, "ld")
			def(LDZpostInc, "ld")
			    def(LDDY, "ldd")
				def(LDDZ, "ldd")

				    def(STS, "sts")

    //st instructions
    def(STX, "st")
	def(STXPLUS, "st")
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

    // --- START X86 Instructions
    def(X86_MOV_CONST, "mov")
	def(X86_MOV_REGS, "mov")
	    def(X86_MOV_LOAD, "mov")
		def(X86_MOV_STORE, "mov")
		    def(X86_CMOVE, "cmove")
			def(X86_XCHG, "xchg")
			    def(X86_PUSH, "push")
				def(X86_POP, "pop")
				    def(X86_ADD, "add")
					def(X86_SUB, "sub")
					    def(X86_IMUL, "imul")
						def(X86_MUL, "mul")
						    def(X86_INC, "inc")
							def(X86_DEC, "dec")
							    def(X86_NEG, "neg")
								def(X86_CMP, "cmp")
								    def(X86_AND, "and")
									def(X86_OR, "or")
									    def(X86_XOR, "xor")
										def(X86_NOT, "not")
	[X86_SHL] = "shl",
	[X86_SHR] = "shr",
	[X86_SAL] = "sal",
	[X86_SAR] = "sar",
	[X86_ROL] = "rol",
	[X86_ROR] = "ror",
										    def(X86_JMP, "jmp")
											def(X86_JE, "je")
											    def(X86_JNE, "jne")
												def(X86_JG, "jg")
												    def(X86_JGE, "jge")
													def(X86_JL, "jl")
													    def(X86_JLE, "jle")
														def(X86_CALL, "call")
														    def(X86_RET, "ret")
															def(X86_NOP, "nop")
    // --- END X86 Instructions

};
