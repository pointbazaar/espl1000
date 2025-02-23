#pragma once

#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

struct Token {
	//Kind 	(the kind of token, for example RPARENS)
	uint16_t kind;

	//Value	(for example "subr" for an identifier)
	uint32_t line_num;

	//it is a pointer, because we do not know
	//how long the token is going to get,
	//and some long identifiers
	//could require more bytes.
	//It is not justified to have emtpy bytes in here
	//in the cases where it is just a small token

	// may be NULL in case of tokens that do
	// not require further distinction
	char* value_ptr;
};

bool token_equals(struct Token* a, struct Token* b);

// @return   string representation of the token
//           caller does not need to free it
char* token_str(struct Token* tk);

struct Token* makeToken(int kind);
// @returns NULL on error
struct Token* makeToken2(int kind, char* value);
struct Token* makeTokenStringConst(char* value);

void freeToken(struct Token* token);
