#ifndef STRUCTMEMBER
#define STRUCTMEMBER

struct TokenList;
struct StructMember;

struct StructMember* makeStructMember(struct TokenList* tokens, bool debug);

#endif
