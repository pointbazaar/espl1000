#pragma once

#include <stdbool.h>
#include <stdio.h>

struct TokenList;

// @returns NULL on error
struct TokenList* read_tokens_from_tokens_file(int fd, char* tokensFile);
