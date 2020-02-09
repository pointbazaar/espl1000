#ifndef SIMPLETYPE
#define SIMPLETYPE

#include <string>

class TokenList;

class SimpleType {

public:

	SimpleType(TokenList tokens);
	SimpleType(std::string typeName);

	std::string typeName;
};
#endif