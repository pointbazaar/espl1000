#ifndef TOKEN
#define TOKEN

#include <string.h>
#include <stdbool.h>

struct Token {

	/*
	 -- FIELDS --
	 - Kind 	(the kind of token, for example RPARENS)
	 - Value	(for example "subr" for an identifier)
	*/
	int lineNumber;
	int kind;
	char* value;
};

bool tokenEquals(struct Token* a, struct Token* b);

struct Token* makeToken(int kind);
struct Token* makeToken2(int kind, char* value);

#endif