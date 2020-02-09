#ifndef ARRAYTYPE
#define ARRAYTYPE

class TokenList;
class Type;

class ArrayType {

public:
	ArrayType(Type* element_type);
	ArrayType(TokenList tokens, bool debug);

	Type* element_type;
};
#endif