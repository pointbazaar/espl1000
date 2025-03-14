#pragma once

#include <stdbool.h>

#include "token/list/TokenList.h"

struct Namespace;

// @returns NULL on error
struct Namespace* invoke_parser(struct TokenList* list, char* filename_display);
