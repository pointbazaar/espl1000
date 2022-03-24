#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/str_ast.h"

#include "tables/lvst/lvst.h"

#define LVST_INITIAL_CAPACITY 10

struct LVST {
	//Local Variable Symbol Table (LVST)
	
	//struct LVST should be opaque,
	//except in this file, for better encapsulation
	
	unsigned int count;
	size_t capacity;
	
	struct LVSTLine** lines;

    size_t stack_frame_size_avr;
};

struct LVST* makeLVST(){
	
	struct LVST* lvst = make(LVST);
	
	lvst->count = 0;
	lvst->capacity = LVST_INITIAL_CAPACITY;
	lvst->lines = malloc(sizeof(struct LVSTLine*)*lvst->capacity);

    lvst->stack_frame_size_avr = 0;
	
	return lvst;
}

void lvst_clear(struct LVST* lvst){

	for(int i = 0; i < lvst->count; i++){
		freeLVSTLine(lvst->lines[i]);
	}
	free(lvst->lines);
	
	lvst->count = 0;
	lvst->capacity = LVST_INITIAL_CAPACITY;
	lvst->lines = malloc(sizeof(struct LVSTLine*)*lvst->capacity);

    lvst->stack_frame_size_avr = 0;
}

void freeLVST(struct LVST* lvst){
	
	for(int i = 0; i < lvst->count; i++){
		freeLVSTLine(lvst->lines[i]);
	}
	free(lvst->lines);
	free(lvst);
}

void freeLVSTLine(struct LVSTLine* l){
	
	//the struct AssignStmt* l->first_occur
	//is freed later on by other means,
	//as it's memory is not managed in the 
	//context of the local variable symbol table
	
	//l->type is also not freed, 
	//for the same reasons
	
	free(l);
}

void lvst_add(struct LVST* lvst, struct LVSTLine* line){
	
	for(int i = 0; i < lvst->count; i++){
		
		struct LVSTLine* current_line = lvst->lines[i];
		
		if(strcmp(current_line->name, line->name) == 0){
			
			//this local var is already present
			//free the line
			freeLVSTLine(line);
			return;
		}
	}
	
	if(lvst->count >= lvst->capacity){
		
		lvst->capacity *= 2;
		
		const int nbytes = sizeof(struct LVSTLine*) * (lvst->capacity);
		
		lvst->lines = realloc(lvst->lines, nbytes);
	}
	
	lvst->lines[lvst->count] = line;
	lvst->count += 1;

    //calculate the size in the stackframe and update the line
    size_t item_size = 2;
    if(line->type->basic_type != NULL){
        if(line->type->basic_type->simple_type != NULL){
            if(line->type->basic_type->simple_type->primitive_type != NULL){
                //TODO: adjust this to the real size, as not all primitives occupy 1 byte
                item_size = 1;
            }
        }
    }

    line->stack_frame_offset_avr = lvst->stack_frame_size_avr;
    lvst->stack_frame_size_avr += item_size;
}

struct LVSTLine* lvst_get(struct LVST* lvst, char* name){
	
	for(int i = 0; i < lvst->count; i++){
		
		struct LVSTLine* line = lvst->lines[i];
		
		if(strcmp(line->name, name) == 0)
			{ return line; }
	}
	
	printf("[LVST][Error] %s not found in LVST\n", name);
	exit(1);
	return NULL;
}

bool lvst_contains(struct LVST* lvst, char* name){
	
	for(int i = 0; i < lvst->count; i++){
		
		char* lv_name = lvst->lines[i]->name;
		
		if(strcmp(lv_name, name) == 0){ return true; }
	}
	return false;
}

void lvst_print(struct LVST* lvst){
	
	char* fmt = " |% -24s|%-5s| %-24s |\n";
	char* linebig = "------------------------";
	char* line5  = "-----";
	
	printf("Local Variable Symbol Table [LVST]\n");
	printf(fmt, "name", "is_arg", "Type");
	printf(fmt, linebig, line5, linebig);
	for(int i = 0; i < lvst->count; i++){
		struct LVSTLine* line = lvst->lines[i];
		
		printf(fmt,
			   line->name,
			   (line->is_arg) ? "yes" : "no",
			   str_type(line->type)
		);
	}
}

size_t lvst_stack_frame_size_avr(struct LVST* lvst){
    //give the size required for the stack frame.
    return lvst->stack_frame_size_avr;
}

size_t lvst_stack_frame_offset_avr(struct LVST* lvst, char* local_var_name){
    for(int i = 0; i < lvst->count; i++){

        struct LVSTLine* line = lvst->lines[i];
        if(strcmp(line->name, local_var_name) == 0)
            return line->stack_frame_offset_avr;
    }

    printf("fatal error in lvst_stack_frame_offset_avr");
    exit(1);
    return 0;
}