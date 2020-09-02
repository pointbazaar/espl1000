#ifndef LOOP_
#define LOOP_

#include <stdbool.h>
#include "tokens.h"

struct Token** lex(char* clean_source, char* tkn_filename);
struct Token** lex_main(char* tkn_filename, char* input_filename, long input_file_length, bool free_tokens);

#endif
