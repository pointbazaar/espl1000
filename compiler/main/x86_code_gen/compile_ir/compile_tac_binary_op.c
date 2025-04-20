#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "ibuffer/ibuffer_x86.h"
#include "rat/rat.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

static void case_arithmetic(int RAT_SCRATCH_REG, struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
static void case_compare(int RAT_SCRATCH_REG, struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

static void case_shift(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

static void case_cmp_lt(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_ge(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_neq(struct IBuffer* ibu, int rdest, int rsrc);
static void case_cmp_eq(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc);

static int label_counter = 0;

bool compile_tac_binary_op_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	switch (tac_op(tac)) {

		case TAC_OP_ADD:
		case TAC_OP_SUB:
		case TAC_OP_MUL:
		case TAC_OP_DIV:
		case TAC_OP_MOD:
		case TAC_OP_AND:
		case TAC_OP_OR:
		case TAC_OP_XOR:
			case_arithmetic(RAT_SCRATCH_REG, rat, tac, ibu);
			break;

		case TAC_OP_CMP_LT:
		case TAC_OP_CMP_GE:
		case TAC_OP_CMP_EQ:
		case TAC_OP_CMP_NEQ:
			case_compare(RAT_SCRATCH_REG, rat, tac, ibu);
			break;

		case TAC_OP_SHIFT_LEFT:
		case TAC_OP_SHIFT_RIGHT:
			case_shift(rat, tac, ibu);
			break;

		default:
			fprintf(stderr, "%s: unsupported op: %d\n", __func__, tac_op(tac));
			return false;
	}

	return true;
}

static void case_shift(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int rsrc = rat_get_register(rat, tac_arg1(tac));
	const int rdest = rat_get_register(rat, tac_dest(tac));
	const enum SD_REGISTER scratch = rat_scratch_reg(rat);

	char* c;
	if (tac_op(tac) == TAC_OP_SHIFT_LEFT) {
		c = "TAC_BINARY_OP <<";
	} else {
		c = "TAC_BINARY_OP >>";
	}

	char tmp_label[32];
	char end_label[32];
	sprintf(tmp_label, "L%d", make_label());
	sprintf(end_label, "L%d", make_label());

	mov_regs(scratch, rsrc, c);

	label(tmp_label);

	test(scratch, scratch, c);
	jz(end_label, c);

	if (tac_op(tac) == TAC_OP_SHIFT_LEFT) {
		shl(rdest, 1, c);
	} else {
		shr(rdest, 1, c);
	}

	dec(scratch, c);
	jmp(tmp_label, c);
	label(end_label);
}

static void case_arithmetic_add(int RAT_SCRATCH_REG, int rdest, int rsrc, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP + ";

	int reg2 = RAT_SCRATCH_REG;

	mov_const(reg2, 0, c);

	add(rdest, rsrc, c);
}

static void case_arithmetic_sub(int rdest, int rsrc, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP - ";

	sub(rdest, rsrc, c);
}

static void case_arithmetic_xor(int rdest, int rsrc, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP ^";

	xor(rdest, rsrc, c);
}

static void case_arithmetic(int RAT_SCRATCH_REG, struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	//left and right operand should have registers

	int rsrc = rat_get_register(rat, tac_arg1(tac));

	int rdest = rat_get_register(rat, tac_dest(tac));

	switch (tac_op(tac)) {

		case TAC_OP_ADD:
			case_arithmetic_add(RAT_SCRATCH_REG, rdest, rsrc, ibu);
			break;

		case TAC_OP_SUB:
			case_arithmetic_sub(rdest, rsrc, ibu);
			break;

		case TAC_OP_MUL:
			mul(rdest, rsrc, "");
			break;

		case TAC_OP_DIV: {
			char* c = "div";
			// rax is already reserved, we will not corrupt it
			// rdx needs to be preserved
			// rdest = rdest / rsrc
			mov_regs(RAT_SCRATCH_REG, SD_REG_RDX, "div: preserve rdx");

			mov_regs(SD_REG_RAX, rdest, c);
			mov_const(SD_REG_RDX, 0, c);
			div(rsrc, c);
			mov_regs(rdest, SD_REG_RAX, c);

			mov_regs(SD_REG_RDX, RAT_SCRATCH_REG, "div: preserve rdx");
		} break;

		case TAC_OP_MOD: {
			char* c = "mod";
			// rax is already reserved, we will not corrupt it
			// rdx needs to be preserved
			// rdest = rdest % rsrc
			mov_regs(RAT_SCRATCH_REG, SD_REG_RDX, "mod: preserve rdx");

			mov_regs(SD_REG_RAX, rdest, c);
			mov_const(SD_REG_RDX, 0, c);
			div(rsrc, c);
			mov_regs(rdest, SD_REG_RDX, c);

			mov_regs(SD_REG_RDX, RAT_SCRATCH_REG, "mod: preserve rdx");
		} break;

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

static void case_compare(int RAT_SCRATCH_REG, struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	//left and right operand should have registers

	int rsrc = rat_get_register(rat, tac_arg1(tac));

	int rdest = rat_get_register(rat, tac_dest(tac));

	switch (tac_op(tac)) {
		case TAC_OP_CMP_LT: case_cmp_lt(RAT_SCRATCH_REG, ibu, rdest, rsrc); break;
		case TAC_OP_CMP_GE: case_cmp_ge(RAT_SCRATCH_REG, ibu, rdest, rsrc); break;
		case TAC_OP_CMP_EQ: case_cmp_eq(RAT_SCRATCH_REG, ibu, rdest, rsrc); break;
		case TAC_OP_CMP_NEQ: case_cmp_neq(ibu, rdest, rsrc); break;
		default: break;
	}
}

static void case_cmp_lt(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc) {

	char* c = "TAC_BINARY_OP <";
	char Ltrue[20];
	char Lend[20];

	sprintf(Ltrue, "Ltrue%d", label_counter);
	sprintf(Lend, "Lend%d", label_counter++);

	cmp(rdest, rsrc, c);
	jl(Ltrue, c);

	xor(rdest, rdest, c);
	jmp(Lend, c);
	label(Ltrue);

	mov_const(RAT_SCRATCH_REG, 1, c);
	mov_regs(rdest, RAT_SCRATCH_REG, c);

	label(Lend);
}

static void case_cmp_ge_8bit(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc) {
	char* c = "TAC_BINARY_OP >=";
	//5 instructions

	// ldi r16, 1
	// cp rdest, rsrc
	// brge Ltrue
	// ldi r16, 0
	//Ltrue:
	// mov rdest, r16

	char Ltrue[20];
	sprintf(Ltrue, "Ltrue%d", label_counter++);

	mov_const(RAT_SCRATCH_REG, 1, c);

	cmp(rdest, rsrc, c);

	jge(Ltrue, c);
	mov_const(RAT_SCRATCH_REG, 0, c);

	label(Ltrue);
	mov_regs(rdest, RAT_SCRATCH_REG, c);
}

static void case_cmp_ge(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc) {

	case_cmp_ge_8bit(RAT_SCRATCH_REG, ibu, rdest, rsrc);
}

static void case_cmp_neq_8bit(struct IBuffer* ibu, int rdest, int rsrc) {

	char* c = "TAC_BINARY_OP !=";

	//we just subtract the 2 registers,
	//if they were equal, rdest will be 0, meaning false
	//otherwise it will be nonzero, meaning true

	sub(rdest, rsrc, c);
}

static void case_cmp_neq(struct IBuffer* ibu, int rdest, int rsrc) {

	case_cmp_neq_8bit(ibu, rdest, rsrc);
}

static void case_cmp_eq(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc) {

	char* c = "TAC_BINARY_OP ==";

	char Lend[20];
	sprintf(Lend, "Lend%d", label_counter++);

	mov_const(RAT_SCRATCH_REG, 0, c);
	cmp(rdest, rsrc, c);
	jne(Lend, c);

	mov_const(RAT_SCRATCH_REG, 1, c);

	label(Lend);
	mov_regs(rdest, RAT_SCRATCH_REG, c);
}
