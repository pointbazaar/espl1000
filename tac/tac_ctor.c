#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tacbuffer.h"
#include "tac.h"
#include "_struct.h"

static uint32_t temp_count = 0; //temporaries for TAC

static uint32_t current_label_index = TAC_NO_LABEL + 1;

static struct TAC* makeTAC() {
	struct TAC* res = malloc(sizeof(struct TAC));

	if (!res) {
		return NULL;
	}

	*res = (struct TAC){
	    .label_index = TAC_NO_LABEL,
	    .kind = TAC_NOP,
	    .const_value = 0,
	    .dest = 0,
	    .arg1 = 0,
	    .op = TAC_OP_NONE,
	    .param_index = 0,
	    .load_store_width = 8,
	};

	return res;
}

uint32_t make_label() {
	uint32_t res = current_label_index;
	current_label_index++;
	return res;
}
uint32_t make_temp() {
	uint32_t res = temp_count;
	temp_count++;
	return res;
}
struct TAC* makeTACLabel(uint32_t label) {
	struct TAC* t = makeTAC();
	t->kind = TAC_LABEL_INDEXED;
	t->label_index = label;
	return t;
}

struct TAC* makeTACLabelFunction(uint32_t sst_index) {
	struct TAC* t = makeTAC();
	t->kind = TAC_LABEL_FUNCTION;
	t->dest = sst_index;
	return t;
}

struct TAC* makeTACGoto(uint32_t label) {
	struct TAC* t = makeTAC();
	t->kind = TAC_GOTO;
	t->label_index = label;
	return t;
}

struct TAC* makeTACIfGoto(uint32_t tmp_condition, uint32_t label_destination) {
	struct TAC* t = makeTAC();
	t->kind = TAC_IF_GOTO;
	t->arg1 = tmp_condition;
	t->label_index = label_destination;
	return t;
}

struct TAC* makeTACIfCMPGoto(uint32_t tmp1, enum TAC_OP op, uint32_t tmp2, uint32_t label_destination) {
	struct TAC* t = makeTAC();
	t->kind = TAC_IF_CMP_GOTO;
	t->dest = tmp1;
	t->arg1 = tmp2;
	t->op = op;
	t->label_index = label_destination;
	return t;
}

struct TAC* makeTACCopy(uint32_t tdest, uint32_t tsrc) {
	struct TAC* t = makeTAC();
	t->kind = TAC_COPY;
	t->dest = tdest;
	t->arg1 = tsrc;
	return t;
}

struct TAC* makeTACLoadLocalAddr(uint32_t tmp, uint32_t local_index, uint8_t addr_width) {

	assert(addr_width >= 2);
	assert(addr_width <= 8);

	struct TAC* t = makeTAC();
	t->kind = TAC_LOAD_LOCAL_ADDR;
	t->dest = tmp;
	t->arg1 = local_index;
	t->load_store_width = addr_width;
	return t;
}

struct TAC* makeTACLoadFunctionPtr(uint32_t tmp, uint32_t sst_index) {

	struct TAC* t = makeTAC();
	t->kind = TAC_LOAD_FUNCTION_PTR;
	t->dest = tmp;
	t->arg1 = sst_index;
	t->load_store_width = 0;
	return t;
}

struct TAC* makeTACStoreLocal(uint32_t local_index, uint32_t tmp) {
	struct TAC* t = makeTAC();
	t->kind = TAC_STORE_LOCAL;
	t->dest = local_index;
	t->arg1 = tmp;
	return t;
}

struct TAC* makeTACConst(uint32_t tmp, int value) {
	struct TAC* t = makeTAC();
	t->kind = TAC_CONST_VALUE;
	t->dest = tmp;
	t->const_value = value;
	return t;
}

struct TAC* makeTACConst16(uint32_t tmp, int value) {

	struct TAC* t = makeTAC();
	t->kind = TAC_CONST_VALUE;
	t->dest = tmp;
	t->const_value = value;
	return t;
}

struct TAC* makeTACBinOp(uint32_t dest, enum TAC_OP op, uint32_t src) {

	struct TAC* t = makeTAC();

	t->kind = TAC_BINARY_OP;
	t->dest = dest;
	t->arg1 = src;
	t->op = op;

	switch (op) {
		case TAC_OP_ADD:
		case TAC_OP_SUB:
		case TAC_OP_MUL:
		case TAC_OP_XOR:
		case TAC_OP_AND:
		case TAC_OP_OR:
		case TAC_OP_CMP_EQ:
		case TAC_OP_CMP_NEQ:
		case TAC_OP_CMP_LT:
		case TAC_OP_CMP_GE:
		case TAC_OP_SHIFT_LEFT:
		case TAC_OP_SHIFT_RIGHT:
			break;
		default:
			fprintf(stderr, "fatal error in %s. unrecognized TAC_OP: %d\n", __func__, op);
			return NULL;
	}

	return t;
}

struct TAC* makeTACUnaryOp(uint32_t dest, uint32_t src, enum TAC_OP op) {
	struct TAC* t = makeTAC();
	t->kind = TAC_UNARY_OP;
	t->dest = dest;
	t->arg1 = src;
	t->op = op;

	switch (op) {
		case TAC_OP_UNARY_NOT:
		case TAC_OP_UNARY_MINUS:
		case TAC_OP_UNARY_BITWISE_NEG:
			break;
		default:
			fprintf(stderr, "fatal error in makeTACUnaryOp\n");
			return NULL;
	}

	return t;
}

struct TAC* makeTACParam(uint32_t dest, bool push16, uint32_t param_index) {

	struct TAC* t = makeTAC();
	t->kind = TAC_PARAM;
	t->dest = dest;

	t->param_index = param_index;

	if (push16)
		t->const_value = 16;
	else
		t->const_value = 8;

	return t;
}

struct TAC* makeTACCall(uint32_t tmp, uint32_t function_index) {
	struct TAC* t = makeTAC();
	t->kind = TAC_CALL;
	t->dest = tmp;
	t->arg1 = function_index;
	return t;
}

struct TAC* makeTACICall(uint32_t tmp, uint32_t tmp_call) {
	struct TAC* t = makeTAC();
	t->kind = TAC_ICALL;
	t->dest = tmp;
	t->arg1 = tmp_call;
	return t;
}

// @param frame_size    the size of the stackframe in bytes
struct TAC* makeTACSetupStackframe(uint32_t frame_size) {
	struct TAC* t = makeTAC();
	t->kind = TAC_SETUP_STACKFRAME;
	t->const_value = frame_size;
	return t;
}

struct TAC* makeTACSetupSP() {
	struct TAC* t = makeTAC();
	t->kind = TAC_SETUP_SP;
	return t;
}

struct TAC* makeTACNop() {
	struct TAC* t = makeTAC();
	t->kind = TAC_NOP;
	return t;
}

struct TAC* makeTACReturn(uint32_t tmp) {
	struct TAC* t = makeTAC();
	t->kind = TAC_RETURN;
	t->dest = tmp;
	return t;
}

struct TAC* makeTACLoad(uint32_t tmp, uint32_t taddr, uint8_t width) {
	struct TAC* t = makeTAC();
	t->kind = TAC_LOAD;
	t->dest = tmp;
	t->arg1 = taddr;

	assert(width <= 8);
	assert(width >= 1);

	t->const_value = width;
	t->load_store_width = width;
	return t;
}

struct TAC* makeTACStore(uint32_t taddr, uint32_t tmp, uint8_t width) {
	struct TAC* t = makeTAC();
	t->kind = TAC_STORE;
	t->dest = taddr;
	t->arg1 = tmp;

	assert(width <= 8);
	t->load_store_width = width;
	return t;
}
