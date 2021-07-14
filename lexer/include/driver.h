#ifndef DRIVER
#define DRIVER

#include <stdio.h>
#include <stdbool.h>

extern FILE* outFile;

void out(int id, char* str);

void out2(int id, int id2);

void out_plus_plus();
void out_minus_minus();

void lexer_print_help();

struct LexerFlags* handle_arguments(int argc, char** argv);

char* lexer_make_tkn_filename(char* filename, bool debug);

#endif
