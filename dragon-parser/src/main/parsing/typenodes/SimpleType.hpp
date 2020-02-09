#ifndef SIMPLETYPE
#define SIMPLETYPE

#include <string>

class TokenList;

class SimpleType {

public:

	SimpleType(TokenList tokens, bool debug);
	SimpleType(std::string typeName);

	std::string typeName;
};
#endif