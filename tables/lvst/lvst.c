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
};

struct LVST* makeLVST(){
	
	struct LVST* lvst = make(LVST);
	
	lvst->count = 0;
	lvst->capacity = LVST_INITIAL_CAPACITY;
	lvst->lines = exit_malloc(sizeof(struct LVSTLine*)*lvst->capacity);
	
	return lvst;
}

void lvst_clear(struct LVST* lvst){

	struct LVSTLine* prev = NULL;
	
	for(int i = 0; i < lvst->count; i++){
		if(lvst->lines[i] != prev){
			prev = lvst->lines[i];
			free(lvst->lines[i]);
		}
	}
	free(lvst->lines);
	
	lvst->count = 0;
	lvst->capacity = LVST_INITIAL_CAPACITY;
	lvst->lines = exit_malloc(sizeof(struct LVSTLine*)*lvst->capacity);
}

void freeLVST(struct LVST* lvst){
	
	struct LVSTLine* prev = NULL;
	
	for(int i = 0; i < lvst->count; i++){
		if(lvst->lines[i] != prev){
			prev = lvst->lines[i];
			free(lvst->lines[i]);
		}
	}
	free(lvst->lines);
	free(lvst);
}

void lvst_add(struct LVST* lvst, struct LVSTLine* line){
	
	for(int i = 0; i < lvst->count; i++){
		
		struct LVSTLine* current_line = lvst->lines[i];
		
		if(strcmp(current_line->name, line->name) == 0){
			
			//this local var is already present
			//free the line
			free(line);
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
}

struct LVSTLine* lvst_get(struct LVST* lvst, char* name){
	
	for(int i = 0; i < lvst->count; i++){
		
		struct LVSTLine* line = lvst->lines[i];
		
		if(strcmp(line->name, name) == 0)
			{ return line; }
	}
	
	printf("[LVST][Error] %s not found in LVST\n", name);
	lvst_print(lvst);
	exit(1);
	return NULL;
}

struct LVSTLine* lvst_at(struct LVST* lvst, uint32_t index){
	if(index >= lvst->count){
		printf("[LVST][Error] index %d not found in LVST\n", index);
		lvst_print(lvst);
		exit(1);
	}
	return lvst->lines[index];
}

uint32_t lvst_index_of(struct LVST* lvst, char* name){
	
	for(int i = 0; i < lvst->count; i++){
		
		struct LVSTLine* line = lvst->lines[i];
		if(strcmp(line->name, name) == 0) { return i; }
	}
	
	printf("[LVST][Error] %s not found in LVST\n", name);
	lvst_print(lvst);
	exit(1);
	return 0;
}

bool lvst_contains(struct LVST* lvst, char* name){
	
	for(int i = 0; i < lvst->count; i++){
		
		char* lv_name = lvst->lines[i]->name;
		
		if(strcmp(lv_name, name) == 0){ return true; }
	}
	return false;
}

void lvst_print(struct LVST* lvst){
	
	char* fmt = " |% -24s|%-6s| %-24s |\n";
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

uint32_t lvst_sizeof_type(struct Type* type){

	//sizeof(type) in bytes

	if(type->basic_type == NULL)
		return 2; //type params, pointer types

	if(type->basic_type->subr_type != NULL)
		return 2; //function pointer type

	struct SimpleType* st = type->basic_type->simple_type;

	if(st->struct_type != NULL) return 2;

	struct PrimitiveType* pt = st->primitive_type;

	if(pt->is_char_type || pt->is_bool_type) return 1;

	switch(pt->int_type){

	case INT: case UINT:
	case INT8: case UINT8: return 1;

	case INT16: case UINT16: return 2;
	case INT32: case UINT32: return 4;
	case INT64: case UINT64: return 8;

	default: return 1;

	}
}

size_t lvst_stack_frame_size_avr(struct LVST* lvst){
    //give the size required for the stack frame.
    //(here meaning just the local variables)
    uint32_t sum = 0;
    
    for(int i = 0; i < lvst->count; i++){
		if(lvst->lines[i]->is_arg) continue;
        sum += lvst_sizeof_type(lvst->lines[i]->type);
	}
	
	return sum;
}

size_t lvst_stack_frame_offset_avr(struct LVST* lvst, char* local_var_name){
	
	uint32_t offset = 1;
	
	//we first look at the local vars
    for(int i = 0; i < lvst->count; i++){
		
        struct LVSTLine* line = lvst->lines[i];
        
        if(line->is_arg == true) continue;
        
        if(strcmp(line->name, local_var_name) == 0) return offset;
		
		offset += lvst_sizeof_type(line->type);
    }
    
    offset += 2; //because of the return address
    
    //we then look at the arguments
    for(int i = 0; i < lvst->count; i++){
		
        struct LVSTLine* line = lvst->lines[i];
        
        if(line->is_arg == false) continue;
        
        if(strcmp(line->name, local_var_name) == 0) return offset;
		
		offset += lvst_sizeof_type(line->type);
    }

    printf("fatal error in lvst_stack_frame_offset_avr.");
    printf("did not find local: %s", local_var_name);
    fflush(stdout);
    exit(1);
    return 0;
}
