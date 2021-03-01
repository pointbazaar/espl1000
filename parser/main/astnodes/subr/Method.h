#ifndef METHOD
#define METHOD

#include <stdbool.h>

struct TokenList;
struct Method;

struct Method* makeMethod(struct TokenList* tokens, bool debug);

#endif
