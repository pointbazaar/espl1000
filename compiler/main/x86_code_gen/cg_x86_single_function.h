#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct Method;
struct Ctx;
struct IBuffer;

void emit_create_stack_frame(uint32_t stack_frame_size, FILE* fout);

// @returns false on error
bool compile_and_write_x86_single_function(struct Method* m, struct Ctx* ctx, struct IBuffer* ibu);
