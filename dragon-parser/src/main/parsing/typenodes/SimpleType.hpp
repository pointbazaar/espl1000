#ifndef SIMPLETYPE
#define SIMPLETYPE

class TokenList;

struct SimpleType {
	
	char* typeName;
};

struct SimpleType* makeSimpleType(TokenList* tokens, bool debug);
struct SimpleType* makeSimpleType(char* typeName);

#endif