#pragma once

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include "ast/ast.h"

struct ST;
struct Ctx;
struct Flags;

struct Ctx* ctx_ctor(struct Flags* flags, struct ST* tables);
void ctx_dtor(struct Ctx* ctx);

struct Flags* ctx_flags(struct Ctx* ctx);
struct ST* ctx_tables(struct Ctx* ctx);

//loop compilation related
void ctx_enter_loop(struct Ctx* ctx, uint32_t label_start, uint32_t label_end);

// @returns false on error
bool ctx_exit_loop(struct Ctx* ctx);

// @returns < 0 on error
int32_t ctx_get_label_loop_start(struct Ctx* ctx);

// @returns < 0 on error
int32_t ctx_get_label_loop_end(struct Ctx* ctx);
