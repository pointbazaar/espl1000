#ifndef ASTREADER
#define ASTREADER

#include "ast.h"
#include <stdbool.h>

//this is the only exposed subroutine

struct AST_Whole_Program* readAST(char* filename, bool debug);

#endif
