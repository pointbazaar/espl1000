#pragma once

#include <stdbool.h>

#include "util/ctx.h"
#include "basic_block/basicblock.h"

struct IBuffer;

void emit_asm_x86_basic_block(struct BasicBlock* block, struct Ctx* ctx, struct IBuffer* ibu);
