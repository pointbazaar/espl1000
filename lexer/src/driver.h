#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "token/list/TokenList.h"

void out_nostr(struct TokenList* list, int id);
void out(struct TokenList* list, int id, char* str);
void out_length(struct TokenList* list, int id, char* str, int length);

void out_plus_plus(struct TokenList* list);
void out_minus_minus(struct TokenList* list);

// @returns NULL on error
struct LexerFlags* handle_arguments(int argc, char** argv);

char* lexer_make_tkn_filename(char* filename);
