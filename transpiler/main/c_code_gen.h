#ifndef CCODEGEN
#define CCODEGEN

#include "../../ast/ast.h"
#include "flags.h"

bool transpileAndWrite(char* filename, struct AST_Whole_Program* ast, struct Flags* flags);

#endif
