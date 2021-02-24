#ifndef METHODCALL
#define METHODCALL

#include <stdbool.h>

struct TokenList;
struct MethodCall;

struct MethodCall* makeMethodCall(struct TokenList* tokens,bool debug);

#endif
