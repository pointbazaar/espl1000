#pragma once

#include <stdint.h>
#include <stdbool.h>

struct Method;
struct Ctx;
struct IBuffer;

// @returns false on error
bool compile_single_function(struct Method* m, struct Ctx* ctx, struct IBuffer* ibu);
