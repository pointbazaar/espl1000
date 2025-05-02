#pragma once

#include <stdio.h>

#include <inttypes.h>

#include "ikey.h"

void ibu_write_instr(enum IKEY key, int32_t x1, int32_t x2, int32_t x3, char* str, char* comment, FILE* fout);
