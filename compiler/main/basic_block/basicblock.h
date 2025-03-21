#pragma once

#include <inttypes.h>

#include "tac/tac.h"

struct BasicBlock {

	uint32_t index;

	struct TACBuffer* buffer;

	//pointers to branch destination
	struct BasicBlock* branch_1; //may be NULL //this is the true branch for conditionals
	struct BasicBlock* branch_2; //may be NULL //this is the false / default branch

	bool visited_assign_registers;
	bool visited_emit_asm;
};

struct BasicBlock* basicblock_ctor(uint32_t index);
void basicblock_dtor(struct BasicBlock* block);

void basicblock_print(struct BasicBlock* block, struct Ctx* ctx);

// @returns NULL on error;
struct BasicBlock** basicblock_create_graph(struct TACBuffer* buffer, char* function_name, int* nblocks, struct Ctx* ctx);
