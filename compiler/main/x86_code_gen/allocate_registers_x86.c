#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "liveness/liveness.h"
#include "tables/symtable/symtable.h"

#include "tac/tac.h"
#include "tac/tacbuffer.h"
#include "basic_block/basicblock.h"
#include "rat/rat.h"

#include "allocate_registers_x86.h"

static void allocate_registers_single_tac(struct TAC* t, struct RAT* rat, struct ST* st, struct Liveness* live);

void allocate_registers_basicblocks(struct BasicBlock** graph, size_t nblocks, struct RAT* rat, struct ST* st, struct Liveness* live) {

	for (size_t i = 0; i < nblocks; i++) {
		allocate_registers(graph[i]->buffer, rat, st, live);
	}
}

void allocate_registers(struct TACBuffer* b, struct RAT* rat, struct ST* st, struct Liveness* live) {

	assert(live != NULL);

	for (size_t i = 0; i < tacbuffer_count(b); i++) {
		struct TAC* t = tacbuffer_get(b, i);
		allocate_registers_single_tac(t, rat, st, live);
	}
}

static uint32_t rat_ensure_register_x86(struct RAT* rat, uint32_t tmp, struct Liveness* live) {
	// on x86 we do not have to use register pairs.
	// shoo shoo, the register will be large enough :)

	if (rat_has_register(rat, tmp)) {
		return rat_get_register(rat, tmp);
	}

	// check for overlap with liveness
	const size_t ntemps = liveness_ntemps(live);

	//TODO: overlap also with temporaries which are
	//never used anywhere (and therefore have no liveness).
	//These need a register too but they do not overlap.
	//
	//So that's a bug.

	bool* live_overlap = calloc(ntemps, sizeof(bool));
	for (size_t t = 0; t < ntemps; t++) {
		if (t == tmp) {
			continue;
		}
		live_overlap[t] = liveness_overlaps(live, tmp, t);
	}

	const int32_t reg = rat_find_reg_no_overlap(rat, live_overlap, ntemps);

	assert(reg >= 0);

	free(live_overlap);

	rat_occupy(rat, reg, tmp, false);

	return reg;
}

static void allocate_registers_single_tac(struct TAC* t, struct RAT* rat, struct ST* st, struct Liveness* live) {

	struct LVST* lvst = st->lvst;
	struct SST* sst = st->sst;

	switch (t->kind) {

		case TAC_CONST_VALUE:
		case TAC_BINARY_OP_IMMEDIATE:
		case TAC_LOAD_LOCAL_ADDR:
		case TAC_COPY:
		case TAC_BINARY_OP:
		case TAC_UNARY_OP:
		case TAC_LOAD_CONST_ADDR:
		case TAC_LOAD:
		case TAC_LOAD_LOCAL:
		case TAC_CALL:
			rat_ensure_register_x86(rat, t->dest, live);
			break;

		default: break;
	}
}
