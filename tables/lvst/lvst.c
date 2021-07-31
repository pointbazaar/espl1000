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
	lvst->lines = malloc(sizeof(struct LVSTLine*)*lvst->capacity);
	
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
