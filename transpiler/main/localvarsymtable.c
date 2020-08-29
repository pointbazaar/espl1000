#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "localvarsymtable.h"

struct LVST* makeLocalVarSymTable(struct Method* subr){
	
	struct LVST* lvst = malloc(sizeof(struct LVST));
	
	lvst->count = 0;
	lvst->capacity = 10;
	lvst->lines = malloc(sizeof(struct LVSTLine*)*lvst->capacity);
	
	return lvst;
}

void freeLVSTLine(struct LVSTLine* l){
	
	//the struct AssignStmt* l->firstOccur
	//is freed later on by other means,
	//as it's memory is not managed in the 
	//context of the local variable symbol table
	
	free(l);
}

void lvst_add(struct LVST* lvst, struct LVSTLine* line){
	
	//the local var symbol table works as a set
	//with 'name' as the key
	
	for(int i = 0; i < lvst->count; i++){
		
		struct LVSTLine* current_line = lvst->lines[i];
		
		if(strcmp(current_line->name, line->name) == 0){
			
			//this local var is already present
			return;
		}
	}
	
	//add the line
	if(lvst->count >= lvst->capacity){
		
		//resize the lvst
		lvst->capacity *= 2;
		
		lvst->lines = 
			realloc(
				lvst->lines, 
				sizeof(struct LVSTLine*) * (lvst->capacity)
			);
	}
	
	lvst->lines[lvst->count] = line;
	lvst->count += 1;
}

struct LVSTLine* lvst_get(struct LVST* lvst, char* name){
	
	for(int i = 0; i < lvst->count; i++){
		
		struct LVSTLine* line = lvst->lines[i];
		
		if(strcmp(line->name, name) == 0){
			
			return line;
		}
	}
	
	printf("Fatal Error: %s not found in localvarsymtable\n", name);
	exit(1);
	return NULL;
}
