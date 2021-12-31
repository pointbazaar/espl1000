#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "basicblock.h"
#include "ast/ast.h"
#include "tacbuffer.h"

static bool* calculate_leaders(struct TACBuffer* buffer);
static struct BasicBlock** collect_basic_blocks(struct TACBuffer* buffer, uint32_t count, bool* is_leader);

static struct BasicBlock* find_block_from_tac_leader(struct BasicBlock** blocks, uint32_t count_blocks, struct TAC* tac);
static struct BasicBlock* find_block_from_label_index(struct BasicBlock** blocks, uint32_t count_blocks, uint32_t label_index);

void create_edges_basic_block(struct TACBuffer *buffer, uint32_t count, struct BasicBlock **blocks,
                              struct BasicBlock *block);

static void write_dot_file_from_graph(struct BasicBlock **blocks, uint32_t count, char* function_name);

struct BasicBlock* basicblock_create_graph(struct TACBuffer* buffer, char* function_name){

    //determine leaders
    bool* is_leader = calculate_leaders(buffer);

    //count them
    uint32_t count = 0;
    for(size_t i = 0; i < buffer->count; i++) {
        if(is_leader[i])
            count++;
    }

    struct BasicBlock** blocks = collect_basic_blocks(buffer, count, is_leader);

    //we inspect the last statement of each block to create the edges between them
    for(size_t i = 0; i < count; i++) {

        //DEBUG:print the blocks
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

static void write_dot_file_from_graph(struct BasicBlock **blocks, uint32_t count, char* function_name) {

    char fname_buffer[100];
    sprintf(fname_buffer, "%s.dot", function_name);
    FILE* fout = fopen(fname_buffer,"w");

    fprintf(fout, "digraph G {\n");

    //specify monospace font
    const char* str_font = "[fontname = \"monospace\"];\n";
    fprintf(fout, "graph %s node %s edge %s", str_font, str_font, str_font);

    for(uint32_t i = 0; i < count; i++){
        struct BasicBlock* block = blocks[i];

        char* tacbuffer_str = tacbuffer_tostring(block->buffer, true);

        fprintf(fout, "b%d [shape=box label=\"%s\"]\n", i, tacbuffer_str);

        free(tacbuffer_str);

        if(block->branch_1 != NULL)
            fprintf(fout, "b%d -> b%d\n", block->index, block->branch_1->index);

        if(block->branch_2 != NULL)
            fprintf(fout, "b%d -> b%d\n", block->index, block->branch_2->index);
    }

    fprintf(fout, "}\n");

    fclose(fout);

    char cmd_buffer[100];

    sprintf(cmd_buffer,"dot -Tpng %s.dot > %s.png", function_name,function_name);
    system(cmd_buffer);
}

void create_edges_basic_block(struct TACBuffer *buffer, uint32_t count, struct BasicBlock **blocks,
                              struct BasicBlock *block) {
    struct TAC* last = block->buffer->buffer[block->buffer->count - 1];

    if(last->kind != TAC_IF_GOTO && last->kind != TAC_GOTO){
        return;
    }

    if(last->goto_index != TAC_NO_LABEL){
        //find out the block where the leader has that label_index
        block->branch_1 = find_block_from_label_index(blocks, count, last->goto_index);
    }

    if(last->kind == TAC_IF_GOTO){
        //the next instruction is also a possible branch

        const uint32_t k = tacbuffer_indexof(buffer, last);

        if(k >= buffer->count-1){
            return;
        }

        struct TAC* next = buffer->buffer[k+1];

        //find basic block of next
        block->branch_2 = find_block_from_tac_leader(blocks, count, next);
    }
}

static struct BasicBlock* find_block_from_label_index(struct BasicBlock** blocks, uint32_t count_blocks, uint32_t label_index){

    for(size_t j = 0; j < count_blocks; j++) {
        struct BasicBlock *candidate = blocks[j];
        if(candidate->buffer->buffer[0]->label_index == label_index)
            return candidate;
    }
    return NULL;
}

static struct BasicBlock* find_block_from_tac_leader(struct BasicBlock** blocks, uint32_t count_blocks, struct TAC* tac){

    for(size_t l = 0; l < count_blocks; l++) {
        struct BasicBlock *candidate = blocks[l];

        if(candidate->buffer->buffer[0] == tac){
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
        tacbuffer_append(current->buffer, buffer->buffer[index_tacbuffer++]);

        while(index_tacbuffer < buffer->count && !is_leader[index_tacbuffer]){
            tacbuffer_append(current->buffer, buffer->buffer[index_tacbuffer++]);
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
        .branch_2 = NULL
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
    bool* is_leader = malloc((sizeof(bool))*buffer->count);

    memset(is_leader, false, (sizeof(bool)*buffer->count));
    is_leader[0] = true; //first statement is leader

    for(size_t i = 0; i < buffer->count; i++){

        //any statement that is the target of a conditional or
        //unconditional got is a leader
        struct TAC* current = buffer->buffer[i];
        if(current->kind == TAC_IF_GOTO || current->kind == TAC_GOTO){

            if(current->label_index != TAC_NO_LABEL)
                is_leader[i] = true;
        }

        if(i != 0){
            struct TAC* prev = buffer->buffer[i-1];

            //any statement that immediately follows a goto or conditional
            //got is a leader
            if(prev->kind == TAC_IF_GOTO || prev->kind == TAC_GOTO || prev->kind == TAC_RETURN)
                is_leader[i] = true;
        }
    }

    return is_leader;
}