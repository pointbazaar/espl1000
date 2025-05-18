#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "avr_code_gen/compile_ir/compile_tac.h"

static void case_arithmetic(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
static void case_compare(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);
static void case_shift(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu);

static void case_cmp_ge(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc, bool wide);
static void case_cmp_neq(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc, bool wide);
static void case_cmp_eq(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc, bool wide);
static void case_cmp_lt(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc, bool wide);

static int label_counter = 0;

static char* c = "TAC_BINARY_OP";

bool compile_tac_binary_op(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	switch (tac_op(tac)) {

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

		case TAC_OP_SHIFT_LEFT:
		case TAC_OP_SHIFT_RIGHT:
			case_shift(rat, tac, ibu);
			break;

		default:
			fprintf(stderr, "%s: unhandled case: %d\n", __func__, tac_op(tac));
			return false;
	}

	return true;
}

static void case_arithmetic_add(int RAT_SCRATCH_REG, int rdest, int rsrc, bool dest_wide, bool src_wide, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP + ";

	int reg2 = RAT_SCRATCH_REG;

	if (src_wide)
		reg2 = rsrc + 1;
	else
		avr_ldi(reg2, 0, c);

	avr_add(rdest, rsrc, c);

	if (dest_wide)
		avr_adc(rdest + 1, reg2, c);
}

static void case_arithmetic_sub(int RAT_SCRATCH_REG, int rdest, int rsrc, bool dest_wide, bool src_wide, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP - ";

	if (dest_wide) {
		if (src_wide) {
			avr_sub(rdest, rsrc, c);
			avr_sbc(rdest + 1, rsrc + 1, c);
		} else {
			avr_ldi(RAT_SCRATCH_REG, 0, c);
			avr_sub(rdest, rsrc, c);
			avr_sbc(rdest + 1, RAT_SCRATCH_REG, c);
		}
	} else {
		avr_sub(rdest, rsrc, c);
	}
}

static void case_arithmetic_xor(int RAT_SCRATCH_REG, int rdest, int rsrc, bool dest_wide, bool src_wide, struct IBuffer* ibu) {

	char* c = "TAC_BINARY_OP ^";

	avr_eor(rdest, rsrc, c);

	if (dest_wide) {

		if (src_wide) {
			avr_eor(rdest + 1, rsrc + 1, c);
		} else {
			avr_ldi(RAT_SCRATCH_REG, 0, c);
			avr_eor(rdest + 1, RAT_SCRATCH_REG, c);
		}
	}
}

static void case_shift(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	assert(tac_op(tac) == TAC_OP_SHIFT_LEFT || tac_op(tac) == TAC_OP_SHIFT_RIGHT);

	const int rsrc = rat_get_register(rat, tac_arg1(tac));
	const int rdest = rat_get_register(rat, tac_dest(tac));
	const bool dest_wide = rat_is_wide(rat, tac_dest(tac));

	char tmp_label[32];
	sprintf(tmp_label, "L%d", make_label());

	char* c;
	if (tac_op(tac) == TAC_OP_SHIFT_LEFT) {
		c = "TAC_BINARY_OP <<";
	} else {
		c = "TAC_BINARY_OP >>";
	}

	avr_label(tmp_label);

	if (tac_op(tac) == TAC_OP_SHIFT_LEFT) {

		if (dest_wide) {
			avr_lsl(rdest + 1, c);
			avr_rol(rdest, c);
		} else {
			avr_lsl(rdest, c);
		}
	} else if (tac_op(tac) == TAC_OP_SHIFT_RIGHT) {

		if (dest_wide) {
			avr_lsr(rdest + 1, c);
			avr_ror(rdest, c);
		} else {
			avr_lsr(rdest, c);
		}
	}

	avr_dec(rsrc, c);
	avr_brne(tmp_label, c);
}

static void case_arithmetic(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);
	//left and right operand should have registers

	int rsrc = rat_get_register(rat, tac_arg1(tac));

	int rdest = rat_get_register(rat, tac_dest(tac));

	bool dest_wide = rat_is_wide(rat, tac_dest(tac));
	bool src_wide = rat_is_wide(rat, tac_arg1(tac));

	switch (tac_op(tac)) {

		case TAC_OP_ADD:
			case_arithmetic_add(RAT_SCRATCH_REG, rdest, rsrc, dest_wide, src_wide, ibu);
			break;

		case TAC_OP_SUB:
			case_arithmetic_sub(RAT_SCRATCH_REG, rdest, rsrc, dest_wide, src_wide, ibu);
			break;

		case TAC_OP_MUL:
			avr_mul(rdest, rsrc, "");
			//TODO: figure out the case with wide temporaries
			// after MUL, result is stored in r1:r0 (r0 is the low byte)
			avr_mov(rdest, r0, "move multiplication result");
			break;

		case TAC_OP_AND:
			avr_and(rdest, rsrc, "");
			if (dest_wide && src_wide)
				avr_and(rdest + 1, rsrc + 1, "");
			break;

		case TAC_OP_OR:
			avr_or(rdest, rsrc, "");
			if (dest_wide && src_wide)
				avr_or(rdest + 1, rsrc + 1, "");
			break;

		case TAC_OP_XOR:
			case_arithmetic_xor(RAT_SCRATCH_REG, rdest, rsrc, dest_wide, src_wide, ibu);
			break;

		default: break;
	}
}

static void case_compare(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);
	//left and right operand should have registers

	int rsrc = rat_get_register(rat, tac_arg1(tac));

	int rdest = rat_get_register(rat, tac_dest(tac));

	const bool wide1 = rat_is_wide(rat, tac_dest(tac));
	const bool wide2 = rat_is_wide(rat, tac_arg1(tac));

	//TODO: the case where both are of different width
	// has to be handled

	const bool wide = wide1 && wide2;

	switch (tac_op(tac)) {
		case TAC_OP_CMP_LT: case_cmp_lt(RAT_SCRATCH_REG, ibu, rdest, rsrc, wide); break;
		case TAC_OP_CMP_GE: case_cmp_ge(RAT_SCRATCH_REG, ibu, rdest, rsrc, wide); break;
		case TAC_OP_CMP_EQ: case_cmp_eq(RAT_SCRATCH_REG, ibu, rdest, rsrc, wide); break;
		case TAC_OP_CMP_NEQ: case_cmp_neq(RAT_SCRATCH_REG, ibu, rdest, rsrc, wide); break;
		default: break;
	}
}

static void case_cmp_lt(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc, bool wide) {

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

	avr_cp(rdest, rsrc, c);
	if (wide)
		avr_cpc(rdest + 1, rsrc + 1, c);

	avr_brlt(Ltrue, c);

	avr_clr(rdest, c);
	avr_rjmp(Lend, c);
	avr_label(Ltrue);

	avr_ldi(RAT_SCRATCH_REG, 1, c);
	avr_mov(rdest, RAT_SCRATCH_REG, c);

	avr_label(Lend);
}

static void case_cmp_ge_8bit(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc) {
	//5 instructions

	// ldi r16, 1
	// cp rdest, rsrc
	// brge Ltrue
	// ldi r16, 0
	//Ltrue:
	// mov rdest, r16

	char Ltrue[20];
	sprintf(Ltrue, "Ltrue%d", label_counter++);

	avr_ldi(RAT_SCRATCH_REG, 1, c);

	avr_cp(rdest, rsrc, c);

	avr_brge(Ltrue, c);
	avr_ldi(RAT_SCRATCH_REG, 0, c);

	avr_label(Ltrue);
	avr_mov(rdest, RAT_SCRATCH_REG, c);
}

static void case_cmp_ge_16bit(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc) {

	char Ltrue[20];
	char Lfalse[20];
	sprintf(Ltrue, "Ltrue%d", label_counter++);
	sprintf(Lfalse, "Lfalse%d", label_counter++);

	avr_ldi(RAT_SCRATCH_REG, 1, c);

	// compare the upper bytes
	avr_cp(rdest + 1, rsrc + 1, c);
	avr_brlo(Lfalse, c);
	avr_cp(rsrc + 1, rdest + 1, c);
	avr_brlo(Ltrue, c);

	avr_cp(rdest, rsrc, c);
	avr_brge(Ltrue, c);

	avr_label(Lfalse);
	avr_ldi(RAT_SCRATCH_REG, 0, c);

	avr_label(Ltrue);
	avr_mov(rdest, RAT_SCRATCH_REG, c);
}

static void case_cmp_ge(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc, bool wide) {

	if (wide) {
		case_cmp_ge_16bit(RAT_SCRATCH_REG, ibu, rdest, rsrc);
	} else {
		case_cmp_ge_8bit(RAT_SCRATCH_REG, ibu, rdest, rsrc);
	}
}

static void case_cmp_neq_8bit(struct IBuffer* ibu, int rdest, int rsrc) {
	//we just subtract the 2 registers,
	//if they were equal, rdest will be 0, meaning false
	//otherwise it will be nonzero, meaning true

	avr_sub(rdest, rsrc, c);
}

static void case_cmp_neq_16bit(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc) {

	avr_ldi(RAT_SCRATCH_REG, 0, c);
	avr_cpse(rdest, rsrc, c);
	avr_ldi(RAT_SCRATCH_REG, 1, c);
	avr_cpse(rdest + 1, rsrc + 1, c);
	avr_ldi(RAT_SCRATCH_REG, 1, c);
	avr_mov(rdest, RAT_SCRATCH_REG, c);
}

static void case_cmp_neq(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc, bool wide) {

	if (wide)
		case_cmp_neq_16bit(RAT_SCRATCH_REG, ibu, rdest, rsrc);
	else
		case_cmp_neq_8bit(ibu, rdest, rsrc);
}

static void case_cmp_eq(int RAT_SCRATCH_REG, struct IBuffer* ibu, int rdest, int rsrc, bool wide) {

	//we use the scratch register

	//4 instructions
	avr_ldi(RAT_SCRATCH_REG, 1, c);
	avr_cpse(rdest, rsrc, c);
	avr_clr(RAT_SCRATCH_REG, c);

	if (wide) {
		avr_cpse(rdest + 1, rsrc + 1, c);
		avr_clr(RAT_SCRATCH_REG, c);
	}

	avr_mov(rdest, RAT_SCRATCH_REG, c);
}
