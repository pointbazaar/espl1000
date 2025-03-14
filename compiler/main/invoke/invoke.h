#pragma once

#include <stdbool.h>

#include "token/list/TokenList.h"

struct Namespace;

// @returns NULL on error
struct TokenList* invoke_lexer(char* filename, bool write_token_file, bool debug);

// @returns NULL on error
struct Namespace* invoke_parser(struct TokenList* list, char* filename_display);
