#pragma once

#include <stdbool.h>

struct Ctx;
struct IBuffer;

bool emit_asm_avr_single_tac(struct RAT* rat, struct TAC* tac, struct Ctx* ctx, struct IBuffer* ibu);
