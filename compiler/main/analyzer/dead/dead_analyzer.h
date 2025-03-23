#pragma once

/* This is the dead code analyzer
 * it looks for functions not reachable from
 * main. If there is no main, all functions
 * are assumed to be live.
 */

#include "compiler/main/util/ctx.h"

struct AST;

// @returns false on error
bool analyze_dead_code(struct Ctx* ctx, struct AST* ast);
