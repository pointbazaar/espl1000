#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "tac/tacbuffer.h"
#include "ast/ast.h"

#include "basicblock.h"
#include "basicblock_printer.h"

static bool* calculate_leaders(struct TACBuffer* buffer);
static struct BasicBlock** collect_basic_blocks(struct TACBuffer* buffer, uint32_t count, bool* is_leader);

static struct BasicBlock* find_block_from_tac_leader(struct BasicBlock** blocks, uint32_t count_blocks, struct TAC* tac);
static struct BasicBlock* find_block_from_label_index(struct BasicBlock** blocks, uint32_t count_blocks, uint32_t label_index);

void create_edges_basic_block(struct TACBuffer *buffer, uint32_t count, struct BasicBlock **blocks,
                              struct BasicBlock *block);

struct BasicBlock* basicblock_create_graph(struct TACBuffer* buffer, char* function_name){

    //determine leaders
    bool* is_leader = calculate_leaders(buffer);

    //count them
    uint32_t count = 0;
    for(size_t i = 0; i < tacbuffer_count(buffer); i++) {
        if(is_leader[i])
            count++;
    }

    struct BasicBlock** blocks = collect_basic_blocks(buffer, count, is_leader);

    //we inspect the last statement of each block to create the edges between them
    for(size_t i = 0; i < count; i++) {

        //DEBUG:print the blocks
        bool debug = false;
        if(debug)
            basicblock_print(blocks[i]);

        create_edges_basic_block(buffer, count, blocks, blocks[i]);
    }

    //.dot file creation
    //TODO: print the basic blocks to dot file
    //TODO: check -debug flag
    write_dot_file_from_graph(blocks, count, function_name);

    free(is_leader);

    return blocks[0];
}

void create_edges_basic_block(struct TACBuffer *buffer, uint32_t count, struct BasicBlock **blocks,
                              struct BasicBlock *block) {

    struct TAC* last = tacbuffer_get_last(block->buffer);

    if(last->label_index != TAC_NO_LABEL){
        //find out the block where the leader has that label_index
        block->branch_1 = find_block_from_label_index(blocks, count, last->label_index);
    }

    //the next instruction is also a possible branch, if
    //the instruction is not a goto or return
    if(tac_is_unconditional_jump(last))
        return;

    if(tacbuffer_indexof(buffer, last) >= (tacbuffer_count(buffer) - 1))
        return;

    struct TAC* next = tacbuffer_get(buffer, tacbuffer_indexof(buffer, last)+1);

    //find basic block of next
    block->branch_2 = find_block_from_tac_leader(blocks, count, next);
}

static struct BasicBlock* find_block_from_label_index(struct BasicBlock** blocks, uint32_t count_blocks, uint32_t label_index){

    for(size_t j = 0; j < count_blocks; j++) {
        struct BasicBlock *candidate = blocks[j];

        if(tacbuffer_get(candidate->buffer, 0)->kind != TAC_LABEL)
            continue;

        if(tacbuffer_get(candidate->buffer, 0)->label_index == label_index)
            return candidate;
    }
    return NULL;
}

static struct BasicBlock* find_block_from_tac_leader(struct BasicBlock** blocks, uint32_t count_blocks, struct TAC* tac){

    for(size_t l = 0; l < count_blocks; l++) {
        struct BasicBlock *candidate = blocks[l];

        if(tacbuffer_get(candidate->buffer, 0) == tac){
            return candidate;
        }
    }
    return NULL;
}

static struct BasicBlock** collect_basic_blocks(struct TACBuffer* buffer, uint32_t count, bool* is_leader){

    struct BasicBlock** blocks = malloc(sizeof(struct BasicBlock*)* count);

    uint32_t index_tacbuffer = 0;
    for(uint32_t index_blocks = 0; index_blocks < count; index_blocks++){

        blocks[index_blocks] = basicblock_ctor(index_blocks);

        struct BasicBlock* current = blocks[index_blocks];
        //now collect all TACs until next leader

        //append the leader
        tacbuffer_append(current->buffer, tacbuffer_get(buffer, index_tacbuffer++));

        while(index_tacbuffer < tacbuffer_count(buffer) && !is_leader[index_tacbuffer]){
            tacbuffer_append(current->buffer, tacbuffer_get(buffer, index_tacbuffer++));
        }
    }
    return blocks;
}

struct BasicBlock* basicblock_ctor(uint32_t index){
    struct BasicBlock* res = make(BasicBlock);
    *res = (struct BasicBlock){
        .index = index,
        .buffer = tacbuffer_ctor(),
        .branch_1 = NULL,
        .branch_2 = NULL,

        .visited_assign_registers = false,
        .visited_emit_asm = false
    };
    return res;
}

void basicblock_dtor(struct BasicBlock* block){
    tacbuffer_dtor(block->buffer);
    free(block);
}

void basicblock_print(struct BasicBlock* block){

    printf(" -- BLOCK %d --\n", block->index);
    tacbuffer_print(block->buffer);
    printf(" -- --\n\n");
}

static bool* calculate_leaders(struct TACBuffer* buffer){

    //for each TAC, determine if it is a leader
    bool* is_leader = malloc((sizeof(bool))*tacbuffer_count(buffer));

    memset(is_leader, false, (sizeof(bool)*tacbuffer_count(buffer)));
    is_leader[0] = true; //first statement is leader

    for(size_t i = 0; i < tacbuffer_count(buffer); i++){

        //any statement that is the target of a conditional or
        //unconditional goto is a leader
        struct TAC* current = tacbuffer_get(buffer, i);

        //if(current->label_index != TAC_NO_LABEL)
        if(current->kind == TAC_LABEL)
            is_leader[i] = true;

        if(i==0) continue;

        //prev exists
        struct TAC *prev = tacbuffer_get(buffer, i-1);

        //any statement that immediately follows a goto or conditional
        //goto is a leader

        if (prev->kind == TAC_IF_GOTO || prev->kind == TAC_GOTO || prev->kind == TAC_RETURN)
            is_leader[i] = true;
    }

    return is_leader;
}
