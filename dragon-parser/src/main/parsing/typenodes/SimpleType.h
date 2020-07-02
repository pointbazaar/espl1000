#ifndef SIMPLETYPE
#define SIMPLETYPE

#include <stdbool.h>

struct TokenList;

struct SimpleType {
	

	//the typeName is not a 'char*',
	//because this way we have less indirection
	//and hopefully more performance

	//can have at most 31 characters,
	//but this limit is arbitrary
	char typeName[32];
};

struct SimpleType* makeSimpleType2(struct TokenList* tokens, bool debug);
struct SimpleType* makeSimpleType(char* typeName);

#endif