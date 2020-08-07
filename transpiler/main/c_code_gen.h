#ifndef CCODEGEN
#define CCODEGEN

#include "../../ast/ast.h"

void transpileAndWrite(char* filename, struct AST_Whole_Program* ast, bool debug);

#endif
