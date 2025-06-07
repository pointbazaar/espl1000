#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct Method;
struct Ctx;
struct IBuffer;

void emit_create_stack_frame(uint32_t stack_frame_size, FILE* fout);

bool compile_and_write_avr_single_function(struct Method* m, struct Ctx* ctx, struct IBuffer* ibu);
