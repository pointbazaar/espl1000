#pragma once

// @returns NULL on error
// @param[out][count]  count of tokens read
#include <stddef.h>
struct Token** lex(char* source, size_t* out_count);

void free_tokens(struct Token** tokens, unsigned int count);
