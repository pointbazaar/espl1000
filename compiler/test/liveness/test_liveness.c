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

static void make_example(struct BasicBlock*** graph, size_t* nblocks) {
	// create fake tacbuffer
	struct TACBuffer* buf = tacbuffer_ctor();

	// t0 = 999
	tacbuffer_append(buf, makeTACConst(0, 999));
	// t1 = t0
	tacbuffer_append(buf, makeTACCopy(1, 0));
	// t2 = t1
	tacbuffer_append(buf, makeTACCopy(2, 1));
	// return t3
	tacbuffer_append(buf, makeTACReturn(2));

	*nblocks = 1;
	*graph = malloc(sizeof(struct BasicBlock*) * 1);
	struct BasicBlock* block = malloc(sizeof(struct BasicBlock));
	*graph[0] = block;

	block->index = 0;
	block->buffer = buf;
	block->branch_1 = NULL;
	block->branch_2 = NULL;
}

static void testcase_1_block(bool debug) {

	status("1 block, no branches");

	size_t nblocks = 1;
	struct BasicBlock** graph = NULL;
	make_example(&graph, &nblocks);

	struct Liveness* live = liveness_calculate(graph, nblocks);

	if (debug) {
		liveness_print(live);
	}

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

static void testcase_2_blocks(bool debug) {

	status("2 blocks, TODO: use basicblock graph");

	//struct Ctx* ctx = NULL;
	//TODO: populate or make 'basicblock_create_graph'
	//withouth the 'ctx' argument

	//TODO: create fake tacbuffer
	//struct TACBuffer* buf = tacbuffer_ctor();
	//char* function_name = "main";

	//int nblocks;
	//struct BasicBlock** graph = basicblock_create_graph(buf, function_name, &nblocks, ctx);
	//struct BasicBlock* root = graph[0];

	size_t nblocks = 2;
	struct BasicBlock** graph = NULL;
	make_example_2_blocks(&graph, &nblocks);

	assert(graph != NULL);

	struct Liveness* live = liveness_calculate(graph, nblocks);
	assert(live != NULL);

	if (debug) {
		liveness_print(live);
	}

	assert(liveness_overlaps(live, 0, 2) == true);
}

void test_liveness() {

	testcase_1_block(true);
	testcase_2_blocks(true);
}
