#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include "tables/cc/cc.h"
#include "tables/sst/sst.h"
#include "tables/symtable/symtable.h"

#include "token/TokenKeys.h"

#define SST_INITIAL_CAPACITY 10

#define ERR_SAME_NAME "[SST][Error] 2 subroutines with same name\n"

#define ERR_NOT_FOUND "[SST][Error] subroutine not found\n"

struct SST {
	//Subroutine Symbol Table (SST)
	//this struct should be opaque
	//outside this file
	
	unsigned int count;
	size_t capacity;
	
	struct SSTLine** lines;
};

static void sst_resize(struct SST* sst);

struct SST* makeSST(){

	struct SST* sst = make(SST);
	
	sst->capacity = SST_INITIAL_CAPACITY;
	
	const int nbytes = sizeof(struct SSTLine*)*sst->capacity;
	
	sst->count    = 0;
	sst->lines    = malloc(nbytes);
	
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

void sst_fill(struct SST* sst, struct Namespace* ns){
	
	for(int i = 0; i < ns->count_methods; i++){
		
		struct SSTLine* line = makeSSTLine2(ns->methods[i], ns->name);
		
		sst_add(sst, line);
	}
}

void sst_print(struct SST* sst){
	
	char* fmt = "%20s|%20s|%8s|%16s\n";
	
	printf("[SST] Subroutine Symbol Table\n");
	printf(fmt, "namespace", "name", "isLibC", "halts?");
	printf("--------------------|--------------------|--------|----------------\n");
	
	for(int i = 0; i < sst->count; i++){
		
		struct SSTLine* line = sst->lines[i];
		
		char* isLibC = (line->isLibC)?"yes":"no";
		
		char* halt_info = "-";
		
		if(line->halts != HALTS_UNKNOWN){
				
			halt_info = (line->halts == HALTS_ALWAYS)?"halts":"inf-loop";
		}
		
		printf(fmt, line->_namespace, line->name, isLibC, halt_info);
	}
}

void freeSST(struct SST* sst){
	
	for(int i = 0; i < sst->count; i++)
		{ freeSSTLine(sst->lines[i]); }
		
	free(sst->lines);
	free(sst);
}

struct SSTLine* makeSSTLine(
	char* name, 
	char* _namespace,
	struct Type* type, 
	bool isLibC,
	enum HALTS halts
){

	struct SSTLine* line = make(SSTLine);
	
	strncpy(line->name,       name,       DEFAULT_STR_SIZE);
	strncpy(line->_namespace, _namespace, DEFAULT_STR_SIZE);
	
	line->method       = NULL;
	
	line->returnType   = type;
	line->isLibC       = isLibC;
	line->cc           = make_cc();
	
	line->dead         = DEAD_UNKNOWN;
	line->halts        = halts;
	
	line->is_private   = false;
	
	return line;
}

struct SSTLine* makeSSTLine2(struct Method* m, char* _namespace){

	struct SSTLine* line = make(SSTLine);
	
	strncpy(line->name,       m->name,    DEFAULT_STR_SIZE);
	strncpy(line->_namespace, _namespace, DEFAULT_STR_SIZE);
	
	line->method       = m;
	
	line->returnType   = m->returnType;
	line->isLibC       = false;
	line->cc           = make_cc();
	
	line->dead         = DEAD_UNKNOWN;
	line->halts        = HALTS_UNKNOWN;
	
	line->is_private   = has_annotation(m->super.annotations, ANNOT_PRIVATE);
	
	return line;
}

void freeSSTLine(struct SSTLine* l){
	
	if(l->cc != NULL){ free_cc(l->cc); }

	free(l);
}

void sst_add(struct SST* sst, struct SSTLine* line){
	
	if(sst_contains(sst, line->name)){
		
		printf(ERR_SAME_NAME);
		printf("\t%s\n", line->name);
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
		
		if(strcmp(line->name, name) == 0)
			{ return line; }
	}
	
	printf(ERR_NOT_FOUND);
	printf("\t%s\n", name);
	exit(1);
	return NULL;
}

uint32_t sst_size(struct SST* sst){
	return sst->count;
}

struct SSTLine* sst_at(struct SST* sst, uint32_t index){

	return sst->lines[index];
}

bool sst_contains(struct SST* sst, char* name){

	for(int i = 0; i < sst->count; i++){
		if(strcmp(sst->lines[i]->name, name) == 0)
			{return true;}
	}
	
	return false;
}

static void sst_resize(struct SST* sst){
	
	if(sst->count < sst->capacity){ return; }
			
	sst->capacity *= 2;
	
	const int nbytes = sizeof(struct SSTLine*) * (sst->capacity);
	
	sst->lines = realloc(sst->lines, nbytes);
}
