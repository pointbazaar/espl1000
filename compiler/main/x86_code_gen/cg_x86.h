#pragma once

#include <stdbool.h>

struct Flags;
struct Ctx;
struct AST;
struct IBuffer;

void emit_call_main_endloop_x86(struct IBuffer* ibu);

bool prologue_x86(struct Ctx* ctx, struct IBuffer* ibu, struct AST* ast);
