#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#include "ibuffer.h"
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
	sprintf(s, "  %-5s", MNEM[key]);

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

static bool write_middle(enum IKEY key, int64_t x1, int64_t x2, char* str, char* s, int32_t x3) {

	const uint8_t nbytes = x3;
	char* width_strs[] = {
	    "byte", "word", "dword", "qword"};

	char* width_str = NULL;
	switch (x3) {
		case 1: width_str = "byte"; break;
		case 2: width_str = "word"; break;
		case 4: width_str = "dword"; break;
		case 8: width_str = "qword"; break;
	}

	switch (key) {

		//r1, r2
		case ADD:
		case ADC:
		case SUB:
		case AND:
		case OR:
		case EOR:
		case MUL:
		case MULS:
		case MULSU:
		case FMUL:
		case FMULS:
		case FMULSU:
		case CPSE:
		case CP:
		case CPC:
		case MOV:
		case SBC:
			strcat_reg(s, x1);
			strcat(s, ", ");
			strcat_reg(s, x2);
			break;
			//sprintf(s, "r%d, r%d", x1, x2); break;

		//rd, k
		case ADIW:
		case SUBI:
		case ANDI:
		case ORI:
		case CPI:
		case LDI:
		case LDS:
		case IN:
		case SBCI:
			strcat_reg(s, x1);
			strcat(s, ", ");
			strcat_num(s, x2);
			break;
			//sprintf(s, "r%d, %d", x1, x2); break;

		//k, rd
		case OUT:
		case STS:
			sprintf(s, "%d, r%d", x1, x2);
			break;

		//rd
		case COM:
		case NEG:
		case INC:
		case DEC:
		case TST:
		case CLR:
		case SER:
		case PUSH:
		case POP:
		case LSL:
		case LSR:
		case ROL:
		case ROR:
			strcat_reg(s, x1);
			break;
			//sprintf(s, "r%d", x1); break;

		//special using X,Y,Z
		case LDX: sprintf(s, "r%d, X", x1); break;
		case LDXpostInc: sprintf(s, "r%d, X+", x1); break;
		case LDY: sprintf(s, "r%d, Y", x1); break;
		case LDYpostInc: sprintf(s, "r%d, Y+", x1); break;
		case LDZ: sprintf(s, "r%d, Z", x1); break;
		case LDZpostInc: sprintf(s, "r%d, Z+", x1); break;

		case LDDY: sprintf(s, "r%d, Y+%d", x1, x2); break;
		case LDDZ: sprintf(s, "r%d, Z+%d", x1, x2); break;

		case STX: sprintf(s, "X, r%d", x1); break;
		case STXPLUS: sprintf(s, "X+, r%d", x1); break;
		case STY: sprintf(s, "Y, r%d", x1); break;
		case STZ: sprintf(s, "Z, r%d", x1); break;

		case STDY: sprintf(s, "Y+%d, r%d", x1, x2); break;

		//ops with 1 label
		case RJMP:
		case JMP:
		case CALL:
		case BREQ:
		case BRNE:
		case BRLO:
		case BRLT:
		case BRGE:
		case BRPL:
			sprintf(s, "%s", str);
			break;

		//ops without any operands
		case RET:
		case RETI:
		case NOP:
			break;

		// --- START X86
		case X86_MOV_LOAD:
			sprintf(s, "%s, qword [%s]", rat_regname_x86(x1), rat_regname_x86(x2));
			break;
		case X86_MOV_STORE:
			sprintf(s, "[%s], %s", rat_regname_x86(x1), rat_regname_x86(x2));
			break;
		case X86_MOV_LOAD_WIDTH:
			assert(width_str != NULL);
			sprintf(s, "%s, %s [%s]", rat_regname_x86_width(x1, nbytes), width_str, rat_regname_x86(x2));
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
			sprintf(s, "%s", rat_regname_x86(x1));
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
			// --- END X86

		default:
			fprintf(stderr, "instr %d not implemented in ibu_write_instr\n", key);
			return false;
	}
	return true;
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
