#ifndef CTX
#define CTX

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
char* ctx_asm_filename(struct Ctx* ctx);
char* ctx_token_filename(struct Ctx* ctx);
#endif
