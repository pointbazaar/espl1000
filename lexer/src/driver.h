#ifndef DRIVER
#define DRIVER

#include <stdio.h>
#include <stdbool.h>

void out(FILE* outFile, int id, char* str);
void out_length(FILE* outFile, int id, char* str, int length);

void out2(FILE* outFile, int id, int id2);

void out_plus_plus(FILE* outFile);
void out_minus_minus(FILE* outFile);

void lexer_print_help();

struct LexerFlags* handle_arguments(int argc, char** argv);

char* lexer_make_tkn_filename(char* filename);

#endif
