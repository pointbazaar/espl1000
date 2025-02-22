#pragma once

// @returns NULL on error
struct Token** lex(char* source);

void free_tokens(struct Token** tokens, unsigned int count);
