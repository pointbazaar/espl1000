#ifndef CONSTVALUE_H
#define CONSTVALUE_H

#include <stdbool.h>

struct ConstValue;
struct TokenList;

struct ConstValue* makeConstValue(struct TokenList* tokens);


#endif
