#pragma once

#include <stdbool.h>

struct Flags;
struct Ctx;
struct AST;
struct IBuffer;

void emit_call_main_endloop_x86(struct IBuffer* ibu);

bool compile_and_write_x86(struct AST* ast, struct Ctx* ctx);
