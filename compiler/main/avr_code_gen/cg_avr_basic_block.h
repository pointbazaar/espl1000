#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tables/symtable/symtable.h"
#include "ast/ast_declare.h"
#include "util/ctx.h"
#include "basic_block/basicblock.h"

struct IBuffer;

bool emit_asm_avr_basic_block(struct BasicBlock* block, struct Ctx* ctx, struct IBuffer* ibu);
