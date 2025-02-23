#pragma once

#include <stdbool.h>

struct TokenList;
struct Call;

struct Call* makeCall(struct TokenList* tokens);
