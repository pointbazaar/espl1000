#ifndef BASICTYPE_H
#define BASICTYPE_H

#include <stdbool.h>

struct TokenList;
struct BasicType;
struct SimpleType;
struct SubrType;

struct BasicType* makeBasicTypeSimple(struct SimpleType* typeNode);
struct BasicType* makeBasicTypeSubr(struct SubrType* typeNode);
struct BasicType* makeBasicType2(struct TokenList* tokens);

#endif
