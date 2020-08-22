#ifndef TRANSPILER
#define TRANSPILER

#include <stdbool.h>

#include "flags.h"

int main(int argc, char* argv[]);

void check_dg_extension(char* filename);

void invoke_lexer_parser(char* filename, bool debug);

void transpileAndCompile(char* filename, struct Flags* flags);

#endif
