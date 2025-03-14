#pragma once

#include <stdio.h>
#include <stdint.h>
#include "token/list/TokenList.h"

//TODO: get rid of this global
extern uint32_t line_no;

// @returns NULL on error
struct TokenList* lexer_impl(FILE* infile);
