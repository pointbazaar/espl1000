#pragma once

#include <stdbool.h>
struct Namespace;

// @returns < 0 on error
// @returns  valid file descriptor on success
int invoke_lexer(char* filename, bool write_token_file);

struct Namespace* invoke_parser(int tokensFd, char* filename_display);
