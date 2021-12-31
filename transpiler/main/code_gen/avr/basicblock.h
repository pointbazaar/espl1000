#ifndef SMALLDRAGON_TOPLEVEL_BASICBLOCK_H
#define SMALLDRAGON_TOPLEVEL_BASICBLOCK_H

#include <inttypes.h>

#include "tac.h"

struct BasicBlock {

    uint32_t index;

    struct TACBuffer* buffer;

    //pointers to branch destination
    struct BasicBlock* branch_1; //may be NULL
    struct BasicBlock* branch_2; //may be NULL
};

struct BasicBlock* basicblock_ctor(uint32_t index);
void basicblock_dtor(struct BasicBlock* block);

void basicblock_print(struct BasicBlock* block);

struct BasicBlock* basicblock_create_graph(struct TACBuffer* buffer, char* function_name);

#endif
