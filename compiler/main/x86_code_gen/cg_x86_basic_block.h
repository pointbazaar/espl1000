#pragma once

#include <stdbool.h>

#include "util/ctx.h"
#include "rat/rat.h"
#include "basic_block/basicblock.h"

struct IBuffer;

void emit_asm_x86_basic_block(struct BasicBlock* block, struct Ctx* ctx, struct IBuffer* ibu, struct RAT* rat);

void allocate_registers(struct TACBuffer* b, struct RAT* rat, struct ST* st);
