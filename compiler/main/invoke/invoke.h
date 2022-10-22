#ifndef INVOKE_H
#define INVOKE_H

#include <stdbool.h>
struct Namespace;

int invoke_lexer (char* filename);
struct Namespace * invoke_parser(char* filename);

#endif
