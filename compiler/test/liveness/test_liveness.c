#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tac/tacbuffer.h"
#include "liveness/liveness.h"

#include "test_liveness.h"

static void status(char* msg) {
	printf(" - [TEST] Liveness %s\n", msg);
}

void test_liveness_1_block() {

	status("1 block, no branches");

	struct TACBuffer* buf = tacbuffer_ctor();

	// t0 = 999
	tacbuffer_append(buf, makeTACConst(0, 999));
	// t1 = t0
	tacbuffer_append(buf, makeTACCopy(1, 0));
	// t2 = t1
	tacbuffer_append(buf, makeTACCopy(2, 1));
	// return t3
	tacbuffer_append(buf, makeTACReturn(2));

	struct Liveness* live = liveness_calc_tacbuffer(buf);

	//liveness_print(live);

	assert(live != NULL);

	assert(liveness_overlaps(live, 0, 1) == true);
	assert(liveness_overlaps(live, 1, 2) == true);

	// t0 and t2 can share a register :D
	assert(liveness_overlaps(live, 0, 2) == false);
}

static void make_example_2_blocks(struct BasicBlock*** graph, size_t* nblocks) {

	struct TACBuffer* buf0 = tacbuffer_ctor();

	// t0 = 999
	tacbuffer_append(buf0, makeTACConst(0, 999));
	// t1 = t0
	tacbuffer_append(buf0, makeTACCopy(1, 0));
	// t2 = t1
	tacbuffer_append(buf0, makeTACCopy(2, 1));
	// return t3
	tacbuffer_append(buf0, makeTACGoto(0));

	struct TACBuffer* buf1 = tacbuffer_ctor();

	tacbuffer_append(buf1, makeTACLabel(0));
	tacbuffer_append(buf1, makeTACReturn(0));
	tacbuffer_append(buf1, makeTACReturn(2));

	*nblocks = 2;
	*graph = malloc(sizeof(struct BasicBlock*) * 2);
	struct BasicBlock* block0 = malloc(sizeof(struct BasicBlock));
	struct BasicBlock* block1 = malloc(sizeof(struct BasicBlock));

	assert(block0 != NULL);
	assert(block1 != NULL);

	(*graph)[0] = block0;
	(*graph)[1] = block1;

	block0->index = 0;
	block0->buffer = buf0;
	block0->branch_1 = block1;
	block0->branch_2 = NULL;

	block1->index = 1;
	block1->buffer = buf1;
	block1->branch_1 = NULL;
	block1->branch_2 = NULL;
}

void test_liveness_2_block() {

	status("2 blocks");

	size_t nblocks = 2;
	struct BasicBlock** graph = NULL;
	make_example_2_blocks(&graph, &nblocks);

	assert(graph != NULL);

	struct Liveness* live = liveness_calc(graph, nblocks);
	assert(live != NULL);

	//liveness_print(live);

	assert(liveness_overlaps(live, 0, 2) == true);
}
