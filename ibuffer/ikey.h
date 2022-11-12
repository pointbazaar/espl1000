#ifndef IKEY_H
#define IKEY_H


//IKEY meaning Instruction Key
enum IKEY {
	
	//not really an IKEY though we need it anyways
	LABEL,
	
	//taken from the instruction set summary in the Datasheet
	
	//Arithmetic and Logic Instructions
	ADD, ADC, ADIW,
	
	SUB, SUBI,
	SBC,
	SBCI,
	SBIW,
	
	AND, ANDI,
	
	OR, ORI,
	
	EOR,
	COM,
	NEG,
	SBR,
	CBR,
	
	INC, DEC,
	
	TST,
	CLR,
	SER,
	
	MUL, MULS, MULSU,
	
	FMUL, FMULS, FMULSU,
	
	DES,
	
	//Branch Instructions
	RJMP = 100, IJMP, EIJMP, JMP,
	
	RCALL, ICALL, EICALL, CALL,
	RET, RETI,
	
	CPSE = 120,
	CP,
	CPC,
	CPI,
	SBRC,
	SBRS,
	SBIC,
	SBIS,
	BRBS,
	BRBC,
	BREQ,
	BRNE,
	BRCS,
	BRCC,
	BRSH,
	
	BRLO = 150,
	BRMI,
	BRPL,
	BRGE,
	BRLT,
	BRHS,
	BRHC,
	BRTS,
	BRTC,
	BRVS,
	BRVC,
	BRIE,
	BRID,
	
	//Data Transfer Instructions
	MOV = 200, MOVW,
	LDI,
	
	LDS, //load with addr
	
	//LD Instructions
	LDX = 230, 
	LDY, 
	LDZ,
	LDDY, //with displacement
	LDDZ, //with displacement
	
	STS, //store with addr
	
	//ST Instructions
	STX, 
	STY, 
	STZ,
	STDY, //with displacement
	
	LPM = 250, ELPM, SPM,
	
	IN, OUT,
	
	PUSH = 270, POP,
	
	XCH,
	LAS,
	LAC,
	LAT,
	
	//Bit and Bit-test Instructions
	LSL = 300, LSR,
	
	ROL, ROR,
	
	ASR,
	SWAP,
	SBI,
	CBI,
	BST,
	BLD,
	BSET,
	BCLR,
	SEC,
	CLC,
	SEN,
	CLN,
	SEZ,
	CLZ,
	SEI,
	CLI,
	SES,
	CLS,
	SEV,
	CLV,
	SET,
	CLT,
	SEH,
	CLH,
	
	//MCU Control Instructions
	BREAK = 400,
	NOP,
	SLEEP,
	WDR,
};

#endif
