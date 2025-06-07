#pragma once

#include <stdbool.h>

struct Flags;
struct Ctx;
struct AST;
struct IBuffer;

void emit_call_main_endloop(struct IBuffer* ibu);

bool avr_prologue(struct Ctx* ctx, struct IBuffer* ibu);
