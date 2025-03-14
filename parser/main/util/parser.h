#pragma once

#include "token/list/TokenList.h"

struct AST* build_ast(struct TokenList* list, char* filename_display);
struct Namespace* build_namespace(struct TokenList* list, char* filename_display);
