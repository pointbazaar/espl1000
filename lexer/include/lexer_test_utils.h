#ifndef SMALLDRAGON_TOPLEVEL_LEXER_TEST_UTILS_H
#define SMALLDRAGON_TOPLEVEL_LEXER_TEST_UTILS_H


struct Token** lex(char* source);

void free_tokens(struct Token** tokens, unsigned int count);

#endif //SMALLDRAGON_TOPLEVEL_LEXER_TEST_UTILS_H
