#pragma once

#include <stdbool.h>

#include "util/ctx.h"
#include "rat/rat.h"
#include "basic_block/basicblock.h"

struct IBuffer;
struct Liveness;

// @returns false on error
bool allocate_registers_basicblocks(struct BasicBlock** graph, size_t nblocks, struct RAT* rat, struct ST* st, struct Liveness* live);

// @returns false on error
bool allocate_registers(struct TACBuffer* b, struct RAT* rat, struct ST* st, struct Liveness* live);
