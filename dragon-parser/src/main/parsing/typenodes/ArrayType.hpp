#ifndef ARRAYTYPE
#define ARRAYTYPE

class TokenList;
struct Type;

struct ArrayType {

	struct Type* element_type;
};

struct ArrayType* makeArrayType(struct Type* element_type);
struct ArrayType* makeArrayType(TokenList* tokens, bool debug);

#endif