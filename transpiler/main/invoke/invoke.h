#ifndef INVOKE_H
#define INVOKE_H

#include <stdbool.h>
#include "flags/flags.h"

bool invoke_lexer_parser(char* filename);

struct AST* invoke_ast_reader(struct Flags* flags);

#endif
