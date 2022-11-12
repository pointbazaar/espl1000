#ifndef IBUFFER_WRITE_H
#define IBUFFER_WRITE_H

#include <inttypes.h>

#include "ibuffer.h"
#include "ikey.h"

void ibu_write_instr(enum IKEY key, int32_t x1, int32_t x2, int32_t x3, char* str, char* comment, FILE* fout);

#endif
