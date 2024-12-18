#pragma once

// liveness analysis helps to share registers among multiple
// temporary variables (in IR representation).
//
// reference:
// https://lambda.uta.edu/cse5317/notes/node40.html

#include <stdbool.h>
#include "tac/tac.h"
#include "basic_block/basicblock.h"

struct Liveness;

// calculate liveness for a single function
struct Liveness* liveness_calc(struct BasicBlock** graph, size_t nblocks);

// for testing and such. It creates the BasicBlock graph internally.
struct Liveness* liveness_calc_tacbuffer(struct TACBuffer* buf);

// print tables to stdout for debug
void liveness_print(struct Liveness* l);

// This can be used in register allocation.
// If a temporary does not have overlapping liveness with
// the other temporaries already occupying a register,
// they can share it.
// @param l      precomputed liveness information
// @param temp1  temporary 1
// @param temp2  temporary 2
// @returns      true if the liveness of temporaries
//               temp1, temp2 overlap at one point in the graph
bool liveness_overlaps(struct Liveness* live, uint32_t temp1, uint32_t temp2);

// @return              true if stmt uses(reads) 'temp'
// @param live          precomputed liveness data
// @param stmt_index    the statement to look at
// @param tmp           index of the temporary
bool liveness_use(struct Liveness* live, uint32_t stmt_index, uint32_t temp);

// @return              true if stmt defines(writes) 'temp'
// @param live          precomputed liveness data
// @param stmt_index    the statement to look at
// @param tmp           index of the temporary
bool liveness_def(struct Liveness* live, uint32_t stmt_index, uint32_t temp);

// @return              true if 'tmp' is live on an incoming edge
// @param live          precomputed liveness data
// @param stmt_index    the statement to look at
// @param tmp           index of the temporary
bool liveness_in(struct Liveness* live, uint32_t stmt_index, uint32_t temp);

// @return              true if 'tmp' is live on an outgoing edge
// @param live          precomputed liveness data
// @param stmt_index    the statement to look at
// @param tmp           index of the temporary
bool liveness_out(struct Liveness* live, uint32_t stmt_index, uint32_t temp);
