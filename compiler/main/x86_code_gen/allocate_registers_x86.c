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

static bool allocate_registers_single_tac(struct TAC* t, struct RAT* rat, struct Liveness* live);

bool allocate_registers_basicblocks(struct BasicBlock** graph, size_t nblocks, struct RAT* rat, struct ST* st, struct Liveness* live) {

	for (size_t i = 0; i < nblocks; i++) {
		if (!allocate_registers(graph[i]->buffer, rat, st, live)) {
			return false;
		}
	}

	return true;
}

// @returns false on error
bool allocate_registers(struct TACBuffer* b, struct RAT* rat, struct ST* st, struct Liveness* live) {

	(void)st;

	assert(live != NULL);

	for (size_t i = 0; i < tacbuffer_count(b); i++) {
		struct TAC* t = tacbuffer_get(b, i);

		if (!allocate_registers_single_tac(t, rat, live)) {
			return false;
		}
	}

	return true;
}

static int32_t rat_ensure_register_x86(struct RAT* rat, uint32_t tmp, struct Liveness* live) {
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

	if (reg < 0) {
		fprintf(stderr, "%s:%s: could not find register for t%d\n", __FILE__, __func__, tmp);
		rat_print(rat, stderr);
	}

	free(live_overlap);

	if (reg >= 0) {
		rat_occupy(rat, reg, tmp, false);
	}

	return reg;
}

static bool allocate_registers_single_tac(struct TAC* t, struct RAT* rat, struct Liveness* live) {

	if (tac_needs_register(t)) {
		return rat_ensure_register_x86(rat, tac_dest(t), live) >= 0;
	}

	return true;
}
