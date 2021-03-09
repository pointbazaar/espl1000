#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables/sst.h"
#include "tables/symtable.h"

#include "typeinference/util/type_str.h"

#define SST_INITIAL_CAPACITY 10

static void sst_resize(struct SST* sst);

struct SST* makeSST(){

	struct SST* sst = malloc(sizeof(struct SST));
	
	sst->count = 0;
	sst->capacity = SST_INITIAL_CAPACITY;
	sst->lines = malloc(sizeof(struct SSTLine*)*sst->capacity);
	
	return sst;
}

void sst_clear(struct SST* sst){

	for(int i = 0; i < sst->count; i++){
		
		freeSSTLine(sst->lines[i]);
	}
	free(sst->lines);
	
	sst->count = 0;
	sst->capacity = SST_INITIAL_CAPACITY;
	
	sst->lines = malloc(sizeof(struct SSTLine*)*sst->capacity);
}

void sst_fill(struct SST* sst, struct Namespace* ns, bool debug){
	
	if(debug){ printf("sst_fill(...)\n"); }
	
	for(int i = 0; i < ns->count_methods; i++){
		
		struct Method* m = ns->methods[i];
		
		struct SSTLine* line = malloc(sizeof(struct SSTLine));
		
		strncpy(line->name, m->name, DEFAULT_STR_SIZE);
		line->isLibC = false;
		line->returnType = m->returnType;
		
		//DEBUG
		//printf("\tadding '%s' to subroutine symbol table\n", line->name);
		
		sst_add(sst, line);
	}
	
	if(debug){ sst_print(sst); }
}

void sst_print(struct SST* sst){
	
	printf("Subroutine Symbol Table (SST)\n");
	printf("%8s|%8s\n", "name", "isLibC");
	printf("--------|--------\n");
	
	for(int i = 0; i < sst->count; i++){
		struct SSTLine* line = sst->lines[i];
		
		printf("%8s|%8s\n", line->name, (line->isLibC)?"yes":"no");
	}
}

void freeSST(struct SST* sst){
	
	for(int i = 0; i < sst->count; i++){
		freeSSTLine(sst->lines[i]);
	}
	free(sst->lines);
	free(sst);
}

struct SSTLine* makeSSTLine(char* name, struct Type* type, bool isLibC){

	struct SSTLine* line = make(SSTLine);
	
	strncpy(line->name, name, DEFAULT_STR_SIZE);
	
	line->returnType   = type;
	line->isLibC       = isLibC;
	
	return line;
}

void freeSSTLine(struct SSTLine* l){
	
	free(l);
}

void sst_add(struct SST* sst, struct SSTLine* line){
	
	if(sst_contains(sst, line->name)){
		
		char* ERR_SAME_NAME = 
			"Error: 2 subroutines with same name %s\n";
		
		printf(ERR_SAME_NAME, line->name);
		exit(1);
		return;
	}
	
	sst_resize(sst);
	
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
	
	printf("Fatal Error: '%s' not found in subroutine symbol table\n", name);
	exit(1);
	return NULL;
}

bool sst_contains(struct SST* sst, char* name){
	
	for(int i = 0; i < sst->count; i++){
		if(strcmp(sst->lines[i]->name, name) == 0){return true;}
	}
	return false;
}

static void sst_resize(struct SST* sst){
	
	if(sst->count >= sst->capacity){
		
		sst->capacity *= 2;
		
		const int nbytes = 
			sizeof(struct SSTLine*) * (sst->capacity);
		
		sst->lines = realloc(sst->lines, nbytes);
	}
}
