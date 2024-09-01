#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "ibuffer.h"
#include "ibuffer_write.h"

#include "../rat/rat.h"

extern char* MNEM[];

static void write_middle(enum IKEY key, int64_t x1, int64_t x2, char* str, char* s);

static void strcat_reg(char* s, uint8_t reg);
static void strcat_num(char* s, int num);

void ibu_write_instr(enum IKEY key, int32_t x1, int32_t x2, int32_t x3, char* str, char* comment, FILE* f) {

	if (key == LABEL) {
		fprintf(f, "\n%s:\n", str);
		return;
	}

	if (x3 != 0) return;

	fprintf(f, "  "); //indentation

	//write the mnemonic
	fprintf(f, "%-5s ", MNEM[key]);

	size_t l = 42;
	if (str != NULL) {
		l += strlen(str);
	}
	char* s = malloc(l);
	strcpy(s, "");

	write_middle(key, x1, x2, str, s);

	fprintf(f, "%-s", s);

	if (comment != NULL && strcmp(comment, "") != 0) {
		fprintf(f, ";%-30s", comment);
	}

	fprintf(f, "\n");

	free(s);
}

static void write_middle(enum IKEY key, int64_t x1, int64_t x2, char* str, char* s) {

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
		case X86_CALL:
			sprintf(s, "%s", str);
			break;
		case X86_RET:
		case X86_NOP:
			// nothing to do
			break;
			// --- END X86

		default:
			printf("instr %d not implemented in ibu_write_instr\n", key);
			fflush(stdout);
			exit(1);
	}
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
