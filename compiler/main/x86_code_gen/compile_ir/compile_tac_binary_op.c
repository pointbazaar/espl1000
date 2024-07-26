#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

static void case_arithmetic(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
static void case_compare(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

static void case_cmp_lt(struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_ge(struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_neq(struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_eq(struct IBuffer* ibu, int rdest, int rsrc);

static int label_counter = 0;

static char* c = "TAC_BINARY_OP";

static int RAT_SCRATCH_REG = -1;

void compile_tac_binary_op_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	RAT_SCRATCH_REG = rat_scratch_reg(rat);

	switch (tac->op) {

		case TAC_OP_ADD:
		case TAC_OP_SUB:
		case TAC_OP_MUL:
		case TAC_OP_AND:
		case TAC_OP_OR:
		case TAC_OP_XOR:
			case_arithmetic(rat, tac, ibu);
			break;

		case TAC_OP_CMP_LT:
		case TAC_OP_CMP_GE:
		case TAC_OP_CMP_EQ:
		case TAC_OP_CMP_NEQ:
			case_compare(rat, tac, ibu);
			break;

		default:
			break;
	}
}

static void case_arithmetic_add(int rdest, int rsrc, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP + ";

	int reg2 = RAT_SCRATCH_REG;

	ldi(reg2, 0, c);

	add(rdest, rsrc, c);
}

static void case_arithmetic_sub(int rdest, int rsrc, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP - ";

	sub(rdest, rsrc, c);
}

static void case_arithmetic_xor(int rdest, int rsrc, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP ^";

	eor(rdest, rsrc, c);
}

static void case_arithmetic(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	//left and right operand should have registers

	int rsrc = rat_get_register(rat, tac->arg1);

	int rdest = rat_get_register(rat, tac->dest);

	switch (tac->op) {

		case TAC_OP_ADD:
			case_arithmetic_add(rdest, rsrc, ibu);
			break;

		case TAC_OP_SUB:
			case_arithmetic_sub(rdest, rsrc, ibu);
			break;

		case TAC_OP_MUL:
			mul(rdest, rsrc, "");
			break;

		case TAC_OP_AND:
			and(rdest, rsrc, "");
			break;

		case TAC_OP_OR:
			or (rdest, rsrc, "");
			break;

		case TAC_OP_XOR:
			case_arithmetic_xor(rdest, rsrc, ibu);
			break;

		default: break;
	}
}

static void case_compare(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	//left and right operand should have registers

	int rsrc = rat_get_register(rat, tac->arg1);

	int rdest = rat_get_register(rat, tac->dest);

	switch (tac->op) {
		case TAC_OP_CMP_LT: case_cmp_lt(ibu, rdest, rsrc); break;
		case TAC_OP_CMP_GE: case_cmp_ge(ibu, rdest, rsrc); break;
		case TAC_OP_CMP_EQ: case_cmp_eq(ibu, rdest, rsrc); break;
		case TAC_OP_CMP_NEQ: case_cmp_neq(ibu, rdest, rsrc); break;
		default: break;
	}
}

static void case_cmp_lt(struct IBuffer* ibu, int rdest, int rsrc) {

	//5 instructions

	// cp rdest, rsrc
	// brlt Ltrue
	// clr rdest
	// rjmp Lend
	//Ltrue:
	// ldi rscratch, 1
	// mov rdest, rscratch
	//Lend:

	char Ltrue[20];
	char Lend[20];

	sprintf(Ltrue, "Ltrue%d", label_counter);
	sprintf(Lend, "Lend%d", label_counter++);

	cp(rdest, rsrc, c);

	brlt(Ltrue, c);

	clr(rdest, c);
	rjmp(Lend, c);
	label(Ltrue);

	ldi(RAT_SCRATCH_REG, 1, c);
	mov(rdest, RAT_SCRATCH_REG, c);

	label(Lend);
}

static void case_cmp_ge_8bit(struct IBuffer* ibu, int rdest, int rsrc) {
	//5 instructions

	// ldi r16, 1
	// cp rdest, rsrc
	// brge Ltrue
	// ldi r16, 0
	//Ltrue:
	// mov rdest, r16

	char Ltrue[20];
	sprintf(Ltrue, "Ltrue%d", label_counter++);

	ldi(RAT_SCRATCH_REG, 1, c);

	cp(rdest, rsrc, c);

	brge(Ltrue, c);
	ldi(RAT_SCRATCH_REG, 0, c);

	label(Ltrue);
	mov(rdest, RAT_SCRATCH_REG, c);
}

static void case_cmp_ge_16bit(struct IBuffer* ibu, int rdest, int rsrc) {

	char Ltrue[20];
	char Lfalse[20];
	sprintf(Ltrue, "Ltrue%d", label_counter++);
	sprintf(Lfalse, "Lfalse%d", label_counter++);

	ldi(RAT_SCRATCH_REG, 1, c);

	// compare the upper bytes
	cp(rdest + 1, rsrc + 1, c);
	brlo(Lfalse, c);
	cp(rsrc + 1, rdest + 1, c);
	brlo(Ltrue, c);

	cp(rdest, rsrc, c);
	brge(Ltrue, c);

	label(Lfalse);
	ldi(RAT_SCRATCH_REG, 0, c);

	label(Ltrue);
	mov(rdest, RAT_SCRATCH_REG, c);
}

static void case_cmp_ge(struct IBuffer* ibu, int rdest, int rsrc) {

	case_cmp_ge_8bit(ibu, rdest, rsrc);
}

static void case_cmp_neq_8bit(struct IBuffer* ibu, int rdest, int rsrc) {
	//we just subtract the 2 registers,
	//if they were equal, rdest will be 0, meaning false
	//otherwise it will be nonzero, meaning true

	sub(rdest, rsrc, c);
}

static void case_cmp_neq_16bit(struct IBuffer* ibu, int rdest, int rsrc) {

	ldi(RAT_SCRATCH_REG, 0, c);
	cpse(rdest, rsrc, c);
	ldi(RAT_SCRATCH_REG, 1, c);
	cpse(rdest + 1, rsrc + 1, c);
	ldi(RAT_SCRATCH_REG, 1, c);
	mov(rdest, RAT_SCRATCH_REG, c);
}

static void case_cmp_neq(struct IBuffer* ibu, int rdest, int rsrc) {

	case_cmp_neq_8bit(ibu, rdest, rsrc);
}

static void case_cmp_eq(struct IBuffer* ibu, int rdest, int rsrc) {

	//we use the scratch register

	//4 instructions
	ldi(RAT_SCRATCH_REG, 1, c);
	cpse(rdest, rsrc, c);
	clr(RAT_SCRATCH_REG, c);

	mov(rdest, RAT_SCRATCH_REG, c);
}
