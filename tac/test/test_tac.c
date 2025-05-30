#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "tables/lvst/lvst.h"
#include "test.h"

#include "tac/tac.h"
#include "tac/tacbuffer.h"

#include "util/status/status.h"

void test_tac_opt_dest() {

	status("tac_opt_dest");
	struct TAC* t;

	t = makeTACCall(0, 1, 0);
	assert(tac_opt_dest(t) == 1);
	free(t);

	t = makeTACParam(0, 1, true, 0, false);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACSetupStackframe(0, 3);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACSetupSP(0);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACReturn(0, 3);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLabelFunction(0, 9);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACGoto(0, 9);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACIfGoto(0, 8, 0);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACIfCMPGoto(0, 1, TAC_OP_OR, 2, 9);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLabel(0, 0);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLoadLocalAddr(0, 33, 0, 8);
	assert(tac_opt_dest(t) == 33);
	assert(tac_load_store_width(t) == 8);
	free(t);

	t = makeTACStoreLocal(0, 33, 9);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLoad(0, 2, 3, 8);
	assert(tac_opt_dest(t) == 2);
	free(t);

	t = makeTACStore(0, 2, 3, 8);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACBinOp(0, 22, TAC_OP_OR, 3);
	assert(tac_opt_dest(t) == 22);
	free(t);

	t = makeTACUnaryOp(0, 23, 3, TAC_OP_UNARY_NOT);
	assert(tac_opt_dest(t) == 23);
	free(t);

	t = makeTACCopy(0, 1, 0);
	assert(tac_opt_dest(t) == 1);
	free(t);

	t = makeTACConst(0, 9, 384);
	assert(tac_opt_dest(t) == 9);
	free(t);

	t = makeTACNop(0);
	assert(tac_opt_dest(t) == -1);
	free(t);
}

static void assert_str(struct TAC* t, char* expect) {

	struct LVST* lvst = lvst_ctor();
	struct LVSTLine* line = malloc(sizeof(struct LVSTLine));
	asprintf(&line->name, "%s", "x");
	line->is_arg = false;
	lvst_add(lvst, line);

	char* s = tac_tostring(t, NULL, lvst);
	if (strcmp(s, expect) != 0) {
		printf("'%s' !=\n'%s'\n\n", s, expect);
	}
	assert(strcmp(s, expect) == 0);

	free(s);

	lvst_free(lvst);

	free(t);
}

void test_tac_tostring() {

	status("tac_tostring");

	//TODO: missing SST here
	//assert_str(makeTACCall(1, 0), "t1 = call main");

	assert_str(makeTACParam(0, 1, true, 0, false), "param t1 (index 0)");
	assert_str(makeTACSetupStackframe(0, 1), "setup_stackframe 1");
	assert_str(makeTACSetupSP(0), "setup SP");
	assert_str(makeTACReturn(0, 1), "return t1");

	//TODO: missing SST here
	//assert_str(makeTACLabelFunction(1), "main:");

	assert_str(makeTACGoto(0, 1), "goto L1");
	assert_str(makeTACIfGoto(0, 1, 2), "if-goto t1 L2");

	assert_str(makeTACIfCMPGoto(0, 1, TAC_OP_AND, 2, 3), "if t1 & t2 goto L3");
	assert_str(makeTACIfCMPGoto(0, 1, TAC_OP_OR, 2, 3), "if t1 | t2 goto L3");
	assert_str(makeTACIfCMPGoto(0, 1, TAC_OP_CMP_LT, 2, 3), "if t1 < t2 goto L3");
	assert_str(makeTACIfCMPGoto(0, 1, TAC_OP_CMP_GE, 2, 3), "if t1 >= t2 goto L3");
	assert_str(makeTACIfCMPGoto(0, 1, TAC_OP_CMP_EQ, 2, 3), "if t1 == t2 goto L3");
	assert_str(makeTACIfCMPGoto(0, 1, TAC_OP_CMP_NEQ, 2, 3), "if t1 != t2 goto L3");

	assert_str(makeTACLabel(0, 1), "L1:");
	assert_str(makeTACLoadLocalAddr(0, 1, 0, 8), "load t1 = &l0 (x) (8 bytes)");
	assert_str(makeTACStoreLocal(0, 0, 2), "store l0 (x) = t2");
	assert_str(makeTACLoad(0, 2, 1, 2), "t2 = [t1] (2 bytes)");
	assert_str(makeTACStore(0, 2, 1, 2), "[t2] = t1 (2 bytes)");

	assert_str(makeTACBinOp(0, 2, TAC_OP_ADD, 1), "t2 += t1");
	assert_str(makeTACBinOp(0, 2, TAC_OP_SUB, 1), "t2 -= t1");
	assert_str(makeTACBinOp(0, 2, TAC_OP_MUL, 1), "t2 *= t1");
	assert_str(makeTACBinOp(0, 2, TAC_OP_XOR, 1), "t2 ^= t1");
	assert_str(makeTACBinOp(0, 2, TAC_OP_AND, 1), "t2 &= t1");
	assert_str(makeTACBinOp(0, 2, TAC_OP_OR, 1), "t2 |= t1");

	assert_str(makeTACUnaryOp(0, 2, 1, TAC_OP_UNARY_NOT), "t2 = !t1");
	assert_str(makeTACUnaryOp(0, 2, 1, TAC_OP_UNARY_MINUS), "t2 = -t1");
	assert_str(makeTACUnaryOp(0, 2, 1, TAC_OP_UNARY_BITWISE_NEG), "t2 = ~t1");

	assert_str(makeTACCopy(0, 2, 1), "t2 = t1");
	assert_str(makeTACConst(0, 2, 1), "t2 = 1");
	assert_str(makeTACNop(0), "nop");
}
