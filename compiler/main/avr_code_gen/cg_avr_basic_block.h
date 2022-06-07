

#ifndef SMALLDRAGON_TOPLEVEL_CG_AVR_BASIC_BLOCK_H
#define SMALLDRAGON_TOPLEVEL_CG_AVR_BASIC_BLOCK_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tables/symtable/symtable.h"
#include "ast/ast_declare.h"
#include "flags.h"
#include "util/ctx.h"
#include "basic_block/basicblock.h"

void emit_asm_avr_basic_block(struct BasicBlock *block, struct Ctx* ctx, FILE *fout);

#endif