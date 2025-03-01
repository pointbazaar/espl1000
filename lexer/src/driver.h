#pragma once

#include <stdio.h>
#include <stdbool.h>

void out_nostr(FILE* outFile, int id);
void out(FILE* outFile, int id, char* str);
void out_length(FILE* outFile, int id, char* str, int length);

void out2(FILE* outFile, int id, int id2);

void out_plus_plus(FILE* outFile);
void out_minus_minus(FILE* outFile);

// @returns NULL on error
struct LexerFlags* handle_arguments(int argc, char** argv);

char* lexer_make_tkn_filename(char* filename);
