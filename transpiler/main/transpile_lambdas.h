#ifndef TRANSPILE_LAMBDA_H
#define TRANSPILE_LAMBDA_H

#include "ast/ast.h"
#include "tables/symtable/symtable.h"

void transpileLambdas(struct AST* ast, struct ST* st);

#endif
