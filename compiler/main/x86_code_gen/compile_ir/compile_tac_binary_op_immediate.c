#include <stdlib.h>
#include <stdio.h>

#include "rat/rat.h"
#include "tac/tac.h"
#include "x86_code_gen/compile_ir/compile_tac.h"

static void case_tac_op_add(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac);
static void case_tac_op_sub(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac);
static void case_tac_op_and(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac);
static void case_tac_op_or(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac);
static void case_tac_op_xor(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac);
static void case_tac_op_shift_left(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac);
static void case_tac_op_shift_right(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac);

void compile_tac_binary_op_immediate_x86(struct RAT* rat, struct TAC* tac, struct IBuffer* ibu) {

	switch (tac->op) {

		case TAC_OP_ADD: case_tac_op_add(ibu, rat, tac); break;
		case TAC_OP_SUB: case_tac_op_sub(ibu, rat, tac); break;
		case TAC_OP_AND: case_tac_op_and(ibu, rat, tac); break;
		case TAC_OP_OR: case_tac_op_or(ibu, rat, tac); break;
		case TAC_OP_XOR: case_tac_op_xor(ibu, rat, tac); break;
		case TAC_OP_SHIFT_LEFT: case_tac_op_shift_left(ibu, rat, tac); break;
		case TAC_OP_SHIFT_RIGHT: case_tac_op_shift_right(ibu, rat, tac); break;

		default:
			printf("unsupported op in compile_tac_binary_op_immediate\n");
			printf("tac->op = %d\n", tac->op);
			exit(1);
	}
}

static void case_tac_op_shift_left(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac) {

	int dest = rat_get_register(rat, tac->dest);
	const uint16_t immediate = tac->const_value;

	char* c = "TAC_BINARY_OP_IMMEDIATE <<";

	for (int i = 0; i < immediate; i++) {
		//lsl(dest, c);
		//TODO
	}
}

static void case_tac_op_shift_right(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac) {

	int dest = rat_get_register(rat, tac->dest);
	const uint16_t immediate = tac->const_value;

	char* c = "TAC_BINARY_OP_IMMEDIATE >>";

	for (int i = 0; i < immediate; i++) {
		//lsr(dest, c);
		//TODO
	}
}

static void case_tac_op_xor(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac) {

	int dest = rat_get_register(rat, tac->dest);
	const uint16_t immediate = tac->const_value;

	char* c = "TAC_BINARY_OP_IMMEDIATE ^";

	uint8_t low = immediate & 0xff;
	uint8_t high = (immediate & 0xff00) >> 8;

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	mov_const(RAT_SCRATCH_REG, low, c);
	xor(dest, RAT_SCRATCH_REG, c);
}

static void case_tac_op_and(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac) {

	//andi can only operate on high registers

	char* c = "TAC_BINARY_OP_IMMEDIATE &";

	const uint8_t rdest = rat_get_register(rat, tac->dest);
	const uint16_t immediate = tac->const_value;

	uint8_t low = immediate & 0x00ff;
	uint8_t high = (immediate & 0xff00) >> 8;

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	mov_const(RAT_SCRATCH_REG, low, c);
	and(rdest, RAT_SCRATCH_REG, c);
}

static void case_tac_op_or(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac) {

	int dest = rat_get_register(rat, tac->dest);
	const uint16_t immediate = tac->const_value;

	char* c = "TAC_BINARY_OP_IMMEDIATE |";

	uint8_t low = immediate & 0xff;
	uint8_t high = (immediate & 0xff00) >> 8;

	or (dest, low, c);
}

static void case_tac_op_add(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac) {

	int dest = rat_get_register(rat, tac->dest);
	const int16_t immediate = tac->const_value;

	char* c = "TAC_BINARY_OP_IMMEDIATE +";

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	mov_const(RAT_SCRATCH_REG, -immediate, c);
	sub(dest, RAT_SCRATCH_REG, c);
}

static void case_tac_op_sub(struct IBuffer* ibu, struct RAT* rat, struct TAC* tac) {

	int dest = rat_get_register(rat, tac->dest);

	const int16_t immediate = tac->const_value;

	char* c = "TAC_BINARY_OP_IMMEDIATE -";

	const int RAT_SCRATCH_REG = rat_scratch_reg(rat);

	mov_const(RAT_SCRATCH_REG, immediate, c);
	sub(dest, RAT_SCRATCH_REG, c);
}
