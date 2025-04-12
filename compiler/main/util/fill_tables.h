#pragma once

#include <stdbool.h>

struct AST;
struct Ctx;

bool fill_tables(struct AST* ast, struct Ctx* ctx);
