#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "tac/tacbuffer.h"
#include "liveness/liveness.h"

#include "test_liveness.h"

static void status(char* msg) {
	printf(" - [TEST] Liveness 'def' map (%s)\n", msg);
}

void test_liveness_def_map_tac_call() {

	status("TAC_CALL");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACCall(0, 1, 0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_param() {

	status("TAC_PARAM");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 33));
	tacbuffer_append(buf, makeTACParam(0, 1, true, 0, false));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 1, 1) == false);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_return() {

	status("TAC_RETURN");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 33));
	tacbuffer_append(buf, makeTACReturn(0, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 1, 1) == false);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_goto() {

	status("TAC_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 0, 33));
	tacbuffer_append(buf, makeTACGoto(0, 1));
	tacbuffer_append(buf, makeTACLabel(0, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 1, 0) == false);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_if_goto() {

	status("TAC_IF_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 0, 33));
	tacbuffer_append(buf, makeTACIfGoto(0, 0, 1));
	tacbuffer_append(buf, makeTACLabel(0, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 1, 0) == false);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_if_cmp_goto() {

	status("TAC_IF_CMP_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 0, 33));
	tacbuffer_append(buf, makeTACIfCMPGoto(0, 0, TAC_OP_OR, 0, 1));
	tacbuffer_append(buf, makeTACLabel(0, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 1, 0) == false);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_load_local_addr() {

	status("TAC_LOAD_LOCAL_ADDR");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACLoadLocalAddr(0, 1, 0, 2));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_store_local() {

	status("TAC_STORE_LOCAL");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 99));
	tacbuffer_append(buf, makeTACStoreLocal(0, 0, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);
	assert(liveness_def(live, 1, 1) == false);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_load() {

	status("TAC_LOAD");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 0xaf));
	tacbuffer_append(buf, makeTACLoad(0, 2, 1, 2));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);
	assert(liveness_def(live, 0, 2) == false);
	assert(liveness_def(live, 1, 1) == false);
	assert(liveness_def(live, 1, 2) == true);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_store() {

	status("TAC_STORE");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 0xaf));
	tacbuffer_append(buf, makeTACStore(0, 0xaf, 1, 2));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);
	assert(liveness_def(live, 1, 1) == false);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_binary_op() {

	status("TAC_BINARY_OP");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 0xaf));
	tacbuffer_append(buf, makeTACConst(0, 2, 0xaf));
	tacbuffer_append(buf, makeTACBinOp(0, 2, TAC_OP_OR, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);
	assert(liveness_def(live, 0, 2) == false);
	assert(liveness_def(live, 1, 1) == false);
	assert(liveness_def(live, 1, 2) == true);
	assert(liveness_def(live, 2, 1) == false);
	assert(liveness_def(live, 2, 2) == true);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_unary_op() {

	status("TAC_UNARY_OP");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 8));
	tacbuffer_append(buf, makeTACConst(0, 2, 9));
	tacbuffer_append(buf, makeTACUnaryOp(0, 2, 1, TAC_OP_UNARY_NOT));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);
	assert(liveness_def(live, 0, 2) == false);
	assert(liveness_def(live, 1, 1) == false);
	assert(liveness_def(live, 1, 2) == true);
	assert(liveness_def(live, 2, 1) == false);
	assert(liveness_def(live, 2, 2) == true);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_copy() {

	status("TAC_COPY");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 8));
	tacbuffer_append(buf, makeTACCopy(0, 2, 1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);
	assert(liveness_def(live, 0, 2) == false);
	assert(liveness_def(live, 1, 1) == false);
	assert(liveness_def(live, 1, 2) == true);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_const_value() {

	status("'def' map (TAC_CONST_VALUE)");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 8));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}

void test_liveness_def_map_tac_nop() {

	status("TAC_NOP");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 1, 8));
	tacbuffer_append(buf, makeTACNop(0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_def(live, 0, 1) == true);
	assert(liveness_def(live, 1, 1) == false);

	tacbuffer_dtor(buf);
	liveness_dtor(live);
}
