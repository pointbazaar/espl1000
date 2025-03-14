#pragma once

struct LexerFlags;

#include "token/list/TokenList.h"

// @returns NULL on error
struct TokenList* lexer_main(struct LexerFlags* flags);
