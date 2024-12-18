#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "tac/tacbuffer.h"
#include "liveness/liveness.h"

#include "test_liveness.h"

static void status(char* msg) {
	printf(" - [TEST] Liveness 'use' map (%s)\n", msg);
}

void test_liveness_use_map_tac_call() {

	status("TAC_CALL");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACCall(1, 0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 0, 1) == false);
}

void test_liveness_use_map_tac_param() {

	status("TAC_PARAM");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACParam(0, true));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 0, 0) == true);
}

void test_liveness_use_map_tac_return() {

	status("TAC_RETURN");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACReturn(0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 0, 0) == true);
}

void test_liveness_use_map_tac_goto() {

	status("TAC_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACGoto(1));
	tacbuffer_append(buf, makeTACLabel(1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == false);
}

void test_liveness_use_map_tac_if_goto() {

	status("TAC_IF_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACIfGoto(0, 1));
	tacbuffer_append(buf, makeTACLabel(1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == true);
}

void test_liveness_use_map_tac_if_cmp_goto() {

	status("TAC_IF_CMP_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACIfCMPGoto(0, TAC_OP_OR, 0, 1));
	tacbuffer_append(buf, makeTACLabel(1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == true);
}

void test_liveness_use_map_tac_load_local() {

	status("TAC_LOAD_LOCAL");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACLoadLocal(0, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == false);
}

void test_liveness_use_map_tac_load_local_addr() {

	status("TAC_LOAD_LOCAL_ADDR");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACLoadLocalAddr(0, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == false);
}

void test_liveness_use_map_tac_store_local() {

	status("TAC_STORE_LOCAL");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACStoreLocal(0, 0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == true);
}

void test_liveness_use_map_tac_load_const_addr() {

	status("TAC_LOAD_CONST_ADDR");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACLoadConstAddr(0, 0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == false);
}

void test_liveness_use_map_tac_store_const_addr() {

	status("TAC_STORE_CONST_ADDR");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACStoreConstAddr(0xaf, 0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == true);
}

void test_liveness_use_map_tac_load() {

	status("TAC_LOAD");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACLoad(1, 0, 2));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == true);
}

void test_liveness_use_map_tac_store() {

	status("TAC_STORE");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 123));
	tacbuffer_append(buf, makeTACStore(1, 0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == true);
}

void test_liveness_use_map_tac_binary_op() {

	status("TAC_BINARY_OP");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 9));
	tacbuffer_append(buf, makeTACConst(1, 8));
	tacbuffer_append(buf, makeTACBinOp(1, TAC_OP_OR, 0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 2, 0) == true);
	assert(liveness_use(live, 2, 1) == true);
}

void test_liveness_use_map_tac_unary_op() {

	status("TAC_UNARY_OP");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 9));
	tacbuffer_append(buf, makeTACUnaryOp(1, 0, TAC_OP_UNARY_NOT));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == true);
	assert(liveness_use(live, 1, 1) == false);
}

void test_liveness_use_map_tac_copy() {

	status("TAC_COPY");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 9));
	tacbuffer_append(buf, makeTACConst(1, 8));
	tacbuffer_append(buf, makeTACCopy(2, 0));
	tacbuffer_append(buf, makeTACCopy(1, 2));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 2, 0) == true);
	assert(liveness_use(live, 2, 1) == false);
	assert(liveness_use(live, 2, 2) == false);

	assert(liveness_use(live, 3, 0) == false);
	assert(liveness_use(live, 3, 1) == false);
	assert(liveness_use(live, 3, 2) == true);
}

void test_liveness_use_map_tac_const_value() {

	status("TAC_CONST_VALUE");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 9));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 0, 0) == false);
}

void test_liveness_use_map_tac_nop() {

	status("TAC_NOP");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 9));
	tacbuffer_append(buf, makeTACNop());

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == false);
}

void test_liveness_use_map_tac_binary_op_immediate() {

	status("TAC_BINARY_OP_IMMEDIATE");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 9));
	tacbuffer_append(buf, makeTACBinOpImmediate(1, TAC_OP_ADD, 3));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_use(live, 1, 0) == false);
	assert(liveness_use(live, 1, 1) == true);
}
