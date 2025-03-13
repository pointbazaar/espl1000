#pragma once

#include <stdio.h>
#include <stdbool.h>

void out_nostr(int outFd, int id);
void out(int outFd, int id, char* str);
void out_length(int outFd, int id, char* str, int length);

void out2(int outFd, int id, int id2);

void out_plus_plus(int outFd);
void out_minus_minus(int outFd);

// @returns NULL on error
struct LexerFlags* handle_arguments(int argc, char** argv);

char* lexer_make_tkn_filename(char* filename);
