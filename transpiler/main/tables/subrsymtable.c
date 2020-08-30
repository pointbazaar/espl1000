#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "subrsymtable.h"

struct SST* makeSubrSymTable(struct Namespace* ns){
	
	struct SST* sst = malloc(sizeof(struct SST));
	
	sst->count = 0;
	sst->capacity = 10;
	sst->lines = malloc(sizeof(struct SST*)*sst->capacity);
	
	//add all the subroutines from the namespace
	for(int i = 0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		struct SSTLine* line = malloc(sizeof(struct SSTLine));
		
		strncpy(line->name, m->name, 32);
		line->isLibC = false;
		
		//TODO: remove this dummy later
		//and replace with the actual return type name
		strncpy(line->returnType, "Int", 32); 
		
		sst_add(sst, line);
	}
	
	return sst;
}

void freeSSTLine(struct SSTLine* l){
	
	free(l);
}

void sst_add(struct SST* sst, struct SSTLine* line){
	
	//the subroutine symbol table works as a set
	//with 'name' as the key
	
	for(int i = 0; i < sst->count; i++){
		
		struct SSTLine* current_line = sst->lines[i];
		
		if(strcmp(current_line->name, line->name) == 0){
			
			//this subroutine is already present.
			//This is not supposed to happen and means
			//that 2 subroutines exist with the same name.
			//This is a fatal error
			printf("Error: 2 subroutines with name %s\n", line->name);
			exit(1);
			return;
		}
	}
	
	//add the line
	if(sst->count >= sst->capacity){
		
		//resize
		sst->capacity *= 2;
		
		sst->lines = 
			realloc(
				sst->lines, 
				sizeof(struct SSTLine*) * (sst->capacity)
			);
	}
	
	sst->lines[sst->count] = line;
	sst->count += 1;
}

struct SSTLine* sst_get(struct SST* sst, char* name){
	
	for(int i = 0; i < sst->count; i++){
		
		struct SSTLine* line = sst->lines[i];
		
		if(strcmp(line->name, name) == 0){
			
			return line;
		}
	}
	
	printf("Fatal Error: %s not found in subroutine symbol table\n", name);
	exit(1);
	return NULL;
}
