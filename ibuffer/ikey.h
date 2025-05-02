#pragma once

//IKEY meaning Instruction Key
enum IKEY {

	//not really an IKEY though we need it anyways
	LABEL,

	//taken from the instruction set summary in the Datasheet

	//Arithmetic and Logic Instructions
	AVR_ADD,
	AVR_ADC,
	AVR_ADIW,

	AVR_SUB,
	AVR_SUBI,
	AVR_SBC,
	AVR_SBCI,
	AVR_SBIW,

	AVR_AND,
	AVR_ANDI,

	AVR_OR,
	AVR_ORI,

	AVR_EOR,
	AVR_COM,
	AVR_NEG,
	AVR_SBR,
	AVR_CBR,

	AVR_INC,
	AVR_DEC,

	AVR_TST,
	AVR_CLR,
	AVR_SER,

	AVR_MUL,
	AVR_MULS,
	AVR_MULSU,

	AVR_FMUL,
	AVR_FMULS,
	AVR_FMULSU,

	AVR_DES,

	//Branch Instructions
	AVR_RJMP = 100,
	AVR_IJMP,
	AVR_EIJMP,
	AVR_JMP,

	AVR_RCALL,
	AVR_ICALL,
	AVR_EICALL,
	AVR_CALL,
	AVR_RET,
	AVR_RETI,

	AVR_CPSE = 120,
	AVR_CP,
	AVR_CPC,
	AVR_CPI,
	AVR_SBRC,
	AVR_SBRS,
	AVR_SBIC,
	AVR_SBIS,
	AVR_BRBS,
	AVR_BRBC,
	AVR_BREQ,
	AVR_BRNE,
	AVR_BRCS,
	AVR_BRCC,
	AVR_BRSH,

	AVR_BRLO = 150,
	AVR_BRMI,
	AVR_BRPL,
	AVR_BRGE,
	AVR_BRLT,
	AVR_BRHS,
	AVR_BRHC,
	AVR_BRTS,
	AVR_BRTC,
	AVR_BRVS,
	AVR_BRVC,
	AVR_BRIE,
	AVR_BRID,

	//Data Transfer Instructions
	AVR_MOV = 200,
	AVR_MOVW,
	AVR_LDI,

	AVR_LDS, //load with addr

	//LD Instructions
	AVR_LDX = 230,
	AVR_LDXpostInc,
	AVR_LDY,
	AVR_LDYpostInc,
	AVR_LDZ,
	AVR_LDZpostInc,
	AVR_LDDY, //with displacement
	AVR_LDDZ, //with displacement

	AVR_STS, //store with addr

	//ST Instructions
	AVR_STX,
	AVR_STXPLUS,
	AVR_STY,
	AVR_STZ,
	AVR_STDY, //with displacement

	AVR_LPM = 250,
	AVR_ELPM,
	AVR_SPM,

	AVR_IN,
	AVR_OUT,

	AVR_PUSH = 270,
	AVR_POP,

	AVR_XCH,
	AVR_LAS,
	AVR_LAC,
	AVR_LAT,

	//Bit and Bit-test Instructions
	AVR_LSL = 300,
	AVR_LSR,

	AVR_ROL,
	AVR_ROR,

	AVR_ASR,
	AVR_SWAP,
	AVR_SBI,
	AVR_CBI,
	AVR_BST,
	AVR_BLD,
	AVR_BSET,
	AVR_BCLR,
	AVR_SEC,
	AVR_CLC,
	AVR_SEN,
	AVR_CLN,
	AVR_SEZ,
	AVR_CLZ,
	AVR_SEI,
	AVR_CLI,
	AVR_SES,
	AVR_CLS,
	AVR_SEV,
	AVR_CLV,
	AVR_SET,
	AVR_CLT,
	AVR_SEH,
	AVR_CLH,

	//MCU Control Instructions
	AVR_BREAK = 400,
	AVR_NOP = 401,
	AVR_SLEEP = 402,
	AVR_WDR = 403,

	// START --- X86 Instructions --
	X86_MOV_CONST = 404, // mov rdest, value_const
	X86_MOV_REGS = 405, // mov rdest, rsrc
	X86_MOV_LOAD_WIDTH = 408, // e.g. mov dword rdest, [rsrc]
	X86_MOV_STORE_WIDTH = 410, // e.g. movzx qword [rdest], rsrc
	X86_MOV_CONST_SYMBOL = 411, // e.g. mov rax, my_function
	X86_CMOVE = 412,
	X86_XCHG = 413,

	X86_MOVZX_LOAD_WIDTH = 414, // e.g. movzx dword rdest, [rsrc]
	X86_MOVZX_REGS_WIDTH = 415, // e.g. movzx r11, r11b

	X86_PUSH = 420,
	X86_POP = 421,
	X86_PUSHA = 422,
	X86_POPA = 423,

	X86_ADD = 424,
	X86_ADC = 425, // with carry
	X86_SUB = 426,
	X86_SBB = 427, // with borrow

	X86_IMUL = 428, // signed
	X86_MUL = 429, // unsigned
	X86_DIV = 430,

	X86_INC, // ++
	X86_DEC, // --
	X86_NEG,
	X86_CMP,
	X86_CMP_CONST, // compare to constant
	X86_TEST,

	X86_SETE, // set bit if equal

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
