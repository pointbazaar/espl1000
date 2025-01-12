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

	t = makeTACCall(1, 0);
	assert(tac_opt_dest(t) == 1);
	free(t);

	t = makeTACParam(1, true, 0);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACSetupStackframe(3);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACSetupSP();
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACReturn(3);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLabelFunction(9);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACGoto(9);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACIfGoto(8, 0);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACIfCMPGoto(1, TAC_OP_OR, 2, 9);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLabel(0);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLoadLocal(32, 0);
	assert(tac_opt_dest(t) == 32);
	free(t);

	t = makeTACLoadLocalAddr(33, 0);
	assert(tac_opt_dest(t) == 33);
	free(t);

	t = makeTACStoreLocal(33, 9);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLoadConstAddr(39, 0xaf);
	assert(tac_opt_dest(t) == 39);
	free(t);

	t = makeTACStoreConstAddr(0xaf, 3);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACLoad(2, 3, 8);
	assert(tac_opt_dest(t) == 2);
	free(t);

	t = makeTACStore(2, 3, 8);
	assert(tac_opt_dest(t) == -1);
	free(t);

	t = makeTACBinOp(22, TAC_OP_OR, 3);
	assert(tac_opt_dest(t) == 22);
	free(t);

	t = makeTACUnaryOp(23, 3, TAC_OP_UNARY_NOT);
	assert(tac_opt_dest(t) == 23);
	free(t);

	t = makeTACCopy(1, 0);
	assert(tac_opt_dest(t) == 1);
	free(t);

	t = makeTACConst(9, 384);
	assert(tac_opt_dest(t) == 9);
	free(t);

	t = makeTACNop();
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

	struct TAC* t;
	char* s;

	//TODO: missing SST here
	//assert_str(makeTACCall(1, 0), "t1 = call main");

	assert_str(makeTACParam(1, true, 0), "param t1 (index 0)");
	assert_str(makeTACSetupStackframe(1), "setup_stackframe 1");
	assert_str(makeTACSetupSP(), "setup SP");
	assert_str(makeTACReturn(1), "return t1");

	//TODO: missing SST here
	//assert_str(makeTACLabelFunction(1), "main:");

	assert_str(makeTACGoto(1), "goto L1");
	assert_str(makeTACIfGoto(1, 2), "if-goto t1 L2");

	assert_str(makeTACIfCMPGoto(1, TAC_OP_AND, 2, 3), "if t1 & t2 goto L3");
	assert_str(makeTACIfCMPGoto(1, TAC_OP_OR, 2, 3), "if t1 | t2 goto L3");
	assert_str(makeTACIfCMPGoto(1, TAC_OP_CMP_LT, 2, 3), "if t1 < t2 goto L3");
	assert_str(makeTACIfCMPGoto(1, TAC_OP_CMP_GE, 2, 3), "if t1 >= t2 goto L3");
	assert_str(makeTACIfCMPGoto(1, TAC_OP_CMP_EQ, 2, 3), "if t1 == t2 goto L3");
	assert_str(makeTACIfCMPGoto(1, TAC_OP_CMP_NEQ, 2, 3), "if t1 != t2 goto L3");

	assert_str(makeTACLabel(1), "L1:");
	assert_str(makeTACLoadLocal(1, 0), "load t1 = l0 (x)");
	assert_str(makeTACLoadLocalAddr(1, 0), "load t1 = &l0 (x)");
	assert_str(makeTACStoreLocal(0, 2), "store l0 (x) = t2");
	assert_str(makeTACLoadConstAddr(1, 175), "t1 = [175]");
	assert_str(makeTACStoreConstAddr(166, 1), "[166] = t1");
	assert_str(makeTACLoad(2, 1, 2), "t2 = [t1]");
	assert_str(makeTACStore(2, 1, 2), "[t2] = t1");

	assert_str(makeTACBinOp(2, TAC_OP_ADD, 1), "t2 += t1");
	assert_str(makeTACBinOp(2, TAC_OP_SUB, 1), "t2 -= t1");
	assert_str(makeTACBinOp(2, TAC_OP_MUL, 1), "t2 *= t1");
	assert_str(makeTACBinOp(2, TAC_OP_XOR, 1), "t2 ^= t1");
	assert_str(makeTACBinOp(2, TAC_OP_AND, 1), "t2 &= t1");
	assert_str(makeTACBinOp(2, TAC_OP_OR, 1), "t2 |= t1");

	assert_str(makeTACUnaryOp(2, 1, TAC_OP_UNARY_NOT), "t2 = !t1");
	assert_str(makeTACUnaryOp(2, 1, TAC_OP_UNARY_MINUS), "t2 = -t1");
	assert_str(makeTACUnaryOp(2, 1, TAC_OP_UNARY_BITWISE_NEG), "t2 = ~t1");

	assert_str(makeTACCopy(2, 1), "t2 = t1");
	assert_str(makeTACConst(2, 1), "t2 = 1");
	assert_str(makeTACNop(), "nop");
}
