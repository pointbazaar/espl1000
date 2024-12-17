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
struct Liveness* liveness_calculate(struct BasicBlock** graph, size_t nblocks);

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
bool liveness_overlaps(struct Liveness* l, uint32_t temp1, uint32_t temp2);
