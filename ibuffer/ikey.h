#ifndef IKEY_H
#define IKEY_H

//IKEY meaning Instruction Key
enum IKEY {

	//not really an IKEY though we need it anyways
	LABEL,

	//taken from the instruction set summary in the Datasheet

	//Arithmetic and Logic Instructions
	ADD,
	ADC,
	ADIW,

	SUB,
	SUBI,
	SBC,
	SBCI,
	SBIW,

	AND,
	ANDI,

	OR,
	ORI,

	EOR,
	COM,
	NEG,
	SBR,
	CBR,

	INC,
	DEC,

	TST,
	CLR,
	SER,

	MUL,
	MULS,
	MULSU,

	FMUL,
	FMULS,
	FMULSU,

	DES,

	//Branch Instructions
	RJMP = 100,
	IJMP,
	EIJMP,
	JMP,

	RCALL,
	ICALL,
	EICALL,
	CALL,
	RET,
	RETI,

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
	MOV = 200,
	MOVW,
	LDI,

	LDS, //load with addr

	//LD Instructions
	LDX = 230,
	LDXpostInc,
	LDY,
	LDYpostInc,
	LDZ,
	LDZpostInc,
	LDDY, //with displacement
	LDDZ, //with displacement

	STS, //store with addr

	//ST Instructions
	STX,
	STXPLUS,
	STY,
	STZ,
	STDY, //with displacement

	LPM = 250,
	ELPM,
	SPM,

	IN,
	OUT,

	PUSH = 270,
	POP,

	XCH,
	LAS,
	LAC,
	LAT,

	//Bit and Bit-test Instructions
	LSL = 300,
	LSR,

	ROL,
	ROR,

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

	// START --- X86 Instructions --
	X86_MOV_CONST, // mov rdest, value_const
	X86_MOV_REGS, // mov rdest, rsrc
	X86_MOV_LOAD, // mov rdest, [rsrc]
	X86_MOV_STORE, // mov [rdest], rsrc
	X86_MOV_LOAD_WIDTH, // e.g. mov dword rdest, [rsrc]
	X86_MOV_STORE_WIDTH, // e.g. mov qword [rdest], rsrc
	X86_MOV_CONST_SYMBOL, // e.g. mov rax, my_function
	X86_CMOVE,
	X86_XCHG,

	X86_PUSH,
	X86_POP,
	X86_PUSHA,
	X86_POPA,

	X86_ADD,
	X86_ADC, // with carry
	X86_SUB,
	X86_SBB, // with borrow

	X86_IMUL, // signed
	X86_MUL, // unsigned

	X86_INC, // ++
	X86_DEC, // --
	X86_NEG,
	X86_CMP,
	X86_TEST,

	// bitwise logical
	X86_AND,
	X86_OR,
	X86_XOR,
	X86_NOT,

	// shift and rotate instructions
	X86_SAR, // shift arithmetic right
	X86_SHR, // shift right
	X86_SAL, // shift arithmetic left
	X86_SHL, // shift left

	X86_ROR,
	X86_ROL,

	// control flow
	// TODO: add the others
	X86_JMP,
	X86_JE, // ==
	X86_JNE, // !=
	X86_JG, // >
	X86_JGE, // >=
	X86_JL, // <
	X86_JLE, // <=
	X86_JS, // jump if sign (negative)
	X86_JZ, // == 0

	X86_CALL,
	X86_ICALL, // e.g. call rax
	X86_RET,
	X86_ENTER,
	X86_LEAVE,

	X86_INT, // syscall
	X86_SYSCALL, // syscall

	X86_NOP,
	// END   --- X86 Instructions --
};

#endif
