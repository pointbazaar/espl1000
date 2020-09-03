#ifndef BASICTYPEWRAPPED
#define BASICTYPEWRAPPED

#include <stdbool.h>

struct TokenList;
struct BasicTypeWrapped;
struct SimpleType;
struct SubrType;

struct BasicTypeWrapped* makeBasicTypeWrappedSimple(struct SimpleType* typeNode);
struct BasicTypeWrapped* makeBasicTypeWrappedSubr(struct SubrType* typeNode);
struct BasicTypeWrapped* makeBasicTypeWrapped2(struct TokenList* tokens, bool debug);

#endif
