#ifndef CALL_H
#define CALL_H

#include <stdbool.h>

struct TokenList;
struct Call;

struct Call* makeCall(struct TokenList* tokens,bool debug);

#endif
