#ifndef DRIVER
#define DRIVER

#include <stdio.h>
#include <stdbool.h>

FILE* outFile;

void out(int id, char* str);

void out2(int id, int id2);

void lexer_print_help();

struct LexerFlags* handleArguments(int argc, char** argv);

char* lexer_make_tkn_filename(char* filename, bool debug);

#endif
