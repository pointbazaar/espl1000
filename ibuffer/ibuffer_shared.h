#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "ikey.h"

struct Instr;
struct IBuffer;

struct IBuffer* ibu_ctor();

void ibu_dtor(struct IBuffer* ibu);

bool ibu_write(struct IBuffer* ibu, FILE* fout);
bool ibu_write_to_file(struct IBuffer* ibu, char* filename);

// @brief sets the source line numbers for subsequent insertions
// into the IBuffer.
void ibu_set_line_num(struct IBuffer* ibu, uint32_t line_no);

void ibu_push(struct IBuffer* ibu, struct Instr* i);

bool ibu_push4(struct IBuffer* ibu, enum IKEY key, int64_t x1, int64_t x2, int64_t x3, char* label, const char* comment);

//constructions macros
#define ibu_branch(instr, label, comment) ibu_push4(ibu, instr, 0, 0, 0, label, comment)

#define ibu0(instr, comment) ibu_push4(ibu, instr, 0, 0, 0, "", comment)
#define ibu1(instr, x1, comment) ibu_push4(ibu, instr, x1, 0, 0, "", comment)
#define ibu2(instr, x1, x2, comment) ibu_push4(ibu, instr, x1, x2, 0, "", comment)
#define ibu3(instr, x1, x2, x3, comment) ibu_push4(ibu, instr, x1, x2, x3, "", comment)
#define ibu4(instr, x1, x2, x3, s, comment) ibu_push4(ibu, instr, x1, x2, x3, s, comment)
