#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tac/tacbuffer.h"
#include "liveness/liveness.h"

#include "test_liveness.h"

static void status(char* msg) {
	printf(" - [TEST] Liveness 'out' map (%s)\n", msg);
}

void test_liveness_out_map_tac_call() {

	status("TAC_CALL");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACCall(1, 0));
	tacbuffer_append(buf, makeTACReturn(1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_out(live, 0, 1) == true);
	assert(liveness_out(live, 1, 1) == false);

	liveness_dtor(live);
	tacbuffer_dtor(buf);
}

void test_liveness_out_map_tac_param() {

	status("TAC_PARAM");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACParam(1, true, 0, false));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_out(live, 0, 1) == false);

	liveness_dtor(live);
	tacbuffer_dtor(buf);
}

void test_liveness_out_map_tac_return() {

	status("TAC_RETURN");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACReturn(1));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_out(live, 0, 1) == false);

	liveness_dtor(live);
	tacbuffer_dtor(buf);
}

void test_liveness_out_map_tac_goto() {

	status("TAC_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 333));
	tacbuffer_append(buf, makeTACGoto(1));
	tacbuffer_append(buf, makeTACLabel(1));
	tacbuffer_append(buf, makeTACReturn(0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_out(live, 1, 0) == true);

	liveness_dtor(live);
	tacbuffer_dtor(buf);
}

void test_liveness_out_map_tac_if_goto() {

	status("TAC_IF_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 333));
	tacbuffer_append(buf, makeTACIfGoto(0, 1));

	tacbuffer_append(buf, makeTACLabel(1));
	tacbuffer_append(buf, makeTACReturn(0));

	tacbuffer_append(buf, makeTACLabel(2));
	tacbuffer_append(buf, makeTACReturn(0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_out(live, 1, 0) == true);

	liveness_dtor(live);
	tacbuffer_dtor(buf);
}

void test_liveness_out_map_tac_if_cmp_goto() {

	status("TAC_IF_CMP_GOTO");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 333));
	tacbuffer_append(buf, makeTACConst(1, 333));
	tacbuffer_append(buf, makeTACIfCMPGoto(1, TAC_OP_OR, 1, 1));

	tacbuffer_append(buf, makeTACLabel(1));
	tacbuffer_append(buf, makeTACReturn(0));

	tacbuffer_append(buf, makeTACLabel(2));
	tacbuffer_append(buf, makeTACReturn(0));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(liveness_out(live, 2, 0) == true);

	liveness_dtor(live);
	tacbuffer_dtor(buf);
}

void test_liveness_out_map_tac_copy() {

	status("TAC_COPY");

	struct TACBuffer* buf = tacbuffer_ctor();

	tacbuffer_append(buf, makeTACConst(0, 999));
	tacbuffer_append(buf, makeTACCopy(1, 0));
	tacbuffer_append(buf, makeTACCopy(2, 1));
	tacbuffer_append(buf, makeTACReturn(2));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	assert(live != NULL);

	assert(liveness_out(live, 0, 0) == true);
	assert(liveness_out(live, 0, 1) == false);
	assert(liveness_out(live, 0, 2) == false);

	assert(liveness_out(live, 1, 0) == false);
	assert(liveness_out(live, 1, 1) == true);
	assert(liveness_out(live, 1, 2) == false);

	assert(liveness_out(live, 2, 0) == false);
	assert(liveness_out(live, 2, 1) == false);
	assert(liveness_out(live, 2, 2) == true);

	assert(liveness_out(live, 3, 0) == false);
	assert(liveness_out(live, 3, 1) == false);
	assert(liveness_out(live, 3, 2) == false);

	liveness_dtor(live);
	tacbuffer_dtor(buf);
}
