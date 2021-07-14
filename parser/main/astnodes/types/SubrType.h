#ifndef SUBROUTINETYPE
#define SUBROUTINETYPE

#include <stdbool.h>

struct TokenList;
struct Type;
struct SubrType;

struct SubrType* makeSubrType(struct TokenList* tokens);
struct SubrType* makeSubrType2(struct Type* tn, bool hasSideEffects, bool throws);


#endif
