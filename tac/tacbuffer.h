#pragma once

#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <sys/types.h>

#include "compiler/main/cli/flags/flags.h"

struct TACBuffer;
struct TAC;
struct SST;
struct LVST;

//TACBuffer functions
struct TACBuffer* tacbuffer_ctor();
void tacbuffer_dtor(struct TACBuffer* buffer);
void tacbuffer_shallow_dtor(struct TACBuffer* buffer);

void tacbuffer_print(struct TACBuffer* buffer, struct SST* sst, struct LVST* lvst);

// @returns NULL on error
char* tacbuffer_tostring(struct TACBuffer* buffer, bool graphviz, struct SST* sst, struct LVST* lvst);
void tacbuffer_append(struct TACBuffer* buffer, struct TAC* node);

// @returns false on error
bool tacbuffer_set(struct TACBuffer* buffer, size_t index, struct TAC* node);

// @returns NULL on error
struct TAC* tacbuffer_get(struct TACBuffer* buffer, size_t index);
struct TAC* tacbuffer_get_last(struct TACBuffer* buffer);

uint32_t tacbuffer_last_dest(struct TACBuffer* buffer);
size_t tacbuffer_count(struct TACBuffer* buffer);

// @returns -1 if 'tac' could not be found in 'buffer'
// @returns index otherwise
ssize_t tacbuffer_indexof(struct TACBuffer* buffer, struct TAC* tac);

// @param i1 index 1
// @param i2 index 2
// @returns false on error
bool tacbuffer_swap(struct TACBuffer* buffer, uint32_t i1, uint32_t i2);

// @brief applies some simple optimizations to the buffer
// @returns false on error
bool tacbuffer_optimize_reorder(struct TACBuffer* buffer, struct Flags* flags);
