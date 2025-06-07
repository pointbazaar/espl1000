#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#include "ibuffer_avr.h"
#include "ibuffer_write.h"

#include "../rat/rat.h"
#include "../rat/rat_x86.h"

extern char* MNEM[];
// @returns false on error
static bool write_middle(enum IKEY key, int64_t x1, int64_t x2, char* str, char* s, int32_t x3);

static void strcat_reg(char* s, uint8_t reg);
static void strcat_num(char* s, int num);

void ibu_write_instr(enum IKEY key, int32_t x1, int32_t x2, int32_t x3, char* str, char* comment, FILE* f) {

	if (key == LABEL) {
		fprintf(f, "\n%s:\n", str);
		return;
	}

	if (key == NASM_SECTION) {
		fprintf(f, "section %s\n", str);
		return;
	}
	if (key == NASM_GLOBAL) {
		fprintf(f, "global %s\n", str);
		return;
	}
	if (key == NASM_EXTERN) {
		fprintf(f, "extern %s\n", str);
		return;
	}
	if (key == NASM_DB) {
		fprintf(f, "%s\n", str);
		return;
	}

	size_t l = 72;
	if (str != NULL) {
		l += strlen(str);
	}
	if (comment != NULL) {
		l += strlen(comment);
		l += 30; // alignment
		l += 20; // mnemonic
	}
	char* s = malloc(l);
	assert(s != NULL);

	//write the mnemonic with indentation
	sprintf(s, "  %-8s", MNEM[key]);

	write_middle(key, x1, x2, str, s + strlen(s), x3);

	if (comment != NULL && strcmp(comment, "") != 0) {
		// comment alignment
		const size_t current_len = strlen(s);
		for (int i = current_len; i < 25; i++) {
			strcat(s, " ");
		}
		strcat(s, ";");
		strcat(s, comment);
	}

	fprintf(f, "%s\n", s);

	free(s);
}

static bool write_middle_x86(enum IKEY key, int64_t x1, int64_t x2, char* str, char* s, int32_t x3) {
	const uint8_t nbytes = x3;

	char* width_str = NULL;
	switch (x3) {
		case 1: width_str = "byte"; break;
		case 2: width_str = "word"; break;
		case 4: width_str = "dword"; break;
		case 8: width_str = "qword"; break;
	}

	switch (key) {

		case X86_MOV_LOAD_WIDTH:
			assert(width_str != NULL);
			sprintf(s, "%s, %s [%s]", rat_regname_x86_width(x1, nbytes), width_str, rat_regname_x86(x2));
			break;
		case X86_MOVZX_LOAD_WIDTH:
			sprintf(s, "%s, %s [%s]", rat_regname_x86(x1), width_str, rat_regname_x86(x2));
			break;
		case X86_MOVZX_REGS_WIDTH:
			sprintf(s, "%s, %s", rat_regname_x86(x1), rat_regname_x86_width(x2, nbytes));
			break;
		case X86_MOV_STORE_WIDTH:
			assert(width_str != NULL);
			sprintf(s, "%s [%s], %s", width_str, rat_regname_x86(x1), rat_regname_x86_width(x2, nbytes));
			break;
		case X86_MOV_CONST_SYMBOL:
			sprintf(s, "%s, %s", rat_regname_x86(x1), str);
			break;
		case X86_PUSH:
		case X86_POP:
		case X86_INC:
		case X86_DEC:
		case X86_NEG:
		case X86_NOT:
		case X86_DIV:
			sprintf(s, "%s", rat_regname_x86(x1));
			break;
		case X86_SETE:
			sprintf(s, "%s", rat_regname_x86_width(x1, 1));
			break;
		case X86_MOV_CONST:
		case X86_SHL:
		case X86_SHR:
		case X86_SAL:
		case X86_SAR:
		case X86_ROL:
		case X86_ROR:
			sprintf(s, "%s, %ld", rat_regname_x86(x1), x2);
			break;
		case X86_ADD:
		case X86_MUL:
		case X86_IMUL:
		case X86_SUB:
		case X86_CMP:
		case X86_TEST:
		case X86_AND:
		case X86_OR:
		case X86_XOR:
		case X86_MOV_REGS:
		case X86_CMOVE:
			sprintf(s, "%s, %s", rat_regname_x86(x1), rat_regname_x86(x2));
			break;
		case X86_CMP_CONST:
			sprintf(s, "%s, %ld", rat_regname_x86(x1), x2);
			break;
		case X86_JMP:
		case X86_JE:
		case X86_JNE:
		case X86_JG:
		case X86_JGE:
		case X86_JL:
		case X86_JLE:
		case X86_JS:
		case X86_JZ:
		case X86_CALL:
			sprintf(s, "%s", str);
			break;
		case X86_ICALL:
			sprintf(s, "%s", rat_regname_x86(x1));
			break;
		case X86_INT:
			sprintf(s, "%ld", x1);
			break;
		case X86_RET:
		case X86_NOP:
		case X86_SYSCALL:
			// nothing to do
			break;

		default:
			return false;
	}
	return true;
}

static bool write_middle_avr(enum IKEY key, int64_t x1, int64_t x2, char* str, char* s, int32_t x3) {
	(void)x3;
	switch (key) {

		//r1, r2
		case AVR_ADD:
		case AVR_ADC:
		case AVR_SUB:
		case AVR_AND:
		case AVR_OR:
		case AVR_EOR:
		case AVR_MUL:
		case AVR_MULS:
		case AVR_MULSU:
		case AVR_FMUL:
		case AVR_FMULS:
		case AVR_FMULSU:
		case AVR_CPSE:
		case AVR_CP:
		case AVR_CPC:
		case AVR_MOV:
		case AVR_SBC:
			strcat_reg(s, x1);
			strcat(s, ", ");
			strcat_reg(s, x2);
			break;
			//sprintf(s, "r%d, r%d", x1, x2); break;

		//rd, k
		case AVR_ADIW:
		case AVR_SUBI:
		case AVR_ANDI:
		case AVR_ORI:
		case AVR_CPI:
		case AVR_LDI:
		case AVR_LDS:
		case AVR_IN:
		case AVR_SBCI:
			strcat_reg(s, x1);
			strcat(s, ", ");
			strcat_num(s, x2);
			break;
			//sprintf(s, "r%d, %d", x1, x2); break;

		//k, rd
		case AVR_OUT:
		case AVR_STS:
			sprintf(s, "%ld, r%ld", x1, x2);
			break;

		//rd
		case AVR_COM:
		case AVR_NEG:
		case AVR_INC:
		case AVR_DEC:
		case AVR_TST:
		case AVR_CLR:
		case AVR_SER:
		case AVR_PUSH:
		case AVR_POP:
		case AVR_LSL:
		case AVR_LSR:
		case AVR_ROL:
		case AVR_ROR:
			strcat_reg(s, x1);
			break;
			//sprintf(s, "r%d", x1); break;

		//special using X,Y,Z
		case AVR_LDX: sprintf(s, "r%ld, X", x1); break;
		case AVR_LDXpostInc: sprintf(s, "r%ld, X+", x1); break;
		case AVR_LDY: sprintf(s, "r%ld, Y", x1); break;
		case AVR_LDYpostInc: sprintf(s, "r%ld, Y+", x1); break;
		case AVR_LDZ: sprintf(s, "r%ld, Z", x1); break;
		case AVR_LDZpostInc: sprintf(s, "r%ld, Z+", x1); break;

		case AVR_LDDY: sprintf(s, "r%ld, Y+%ld", x1, x2); break;
		case AVR_LDDZ: sprintf(s, "r%ld, Z+%ld", x1, x2); break;

		case AVR_STX: sprintf(s, "X, r%ld", x1); break;
		case AVR_STXPLUS: sprintf(s, "X+, r%ld", x1); break;
		case AVR_STY: sprintf(s, "Y, r%ld", x1); break;
		case AVR_STZ: sprintf(s, "Z, r%ld", x1); break;

		case AVR_STDY: sprintf(s, "Y+%ld, r%ld", x1, x2); break;

		//ops with 1 label
		case AVR_RJMP:
		case AVR_JMP:
		case AVR_CALL:
		case AVR_BREQ:
		case AVR_BRNE:
		case AVR_BRLO:
		case AVR_BRLT:
		case AVR_BRGE:
		case AVR_BRPL:
			sprintf(s, "%s", str);
			break;

		//ops without any operands
		case AVR_RET:
		case AVR_RETI:
		case AVR_NOP:
			break;

		default:
			return false;
	}
	return true;
}

static bool write_middle(enum IKEY key, int64_t x1, int64_t x2, char* str, char* s, int32_t x3) {

	if (write_middle_avr(key, x1, x2, str, s, x3)) {
		return true;
	}
	if (write_middle_x86(key, x1, x2, str, s, x3)) {
		return true;
	}

	fprintf(stderr, "instr %d not implemented in ibu_write_instr\n", key);
	return false;
}

static void strcat_reg(char* s, uint8_t reg) {

	//print XL, XH, YL, ...
	//using their mnemonic names

	char* c = "";
	char arr[8];
	sprintf(arr, "r%d", reg);

	switch (reg) {
		case XL: c = "XL"; break;
		case XH: c = "XH"; break;
		case YL: c = "YL"; break;
		case YH: c = "YH"; break;
		case ZL: c = "ZL"; break;
		case ZH: c = "ZH"; break;

		default: c = (char*)&arr;
	}

	strcat(s, c);
}

static void strcat_num(char* s, int num) {
	char arr[8];
	sprintf(arr, "%d", num);
	strcat(s, (char*)&arr);
}
