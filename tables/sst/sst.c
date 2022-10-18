#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <ast/util/free_ast.h>

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
	
	uint32_t count;
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
	
	sst->count    = 0;
	sst->capacity = SST_INITIAL_CAPACITY;
	
	sst->lines = malloc(sizeof(struct SSTLine*)*sst->capacity);
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
	struct Type* return_type,
	bool isLibC,
	enum HALTS halts,
	bool has_side_effect
){
	struct SSTLine* line = make(SSTLine);
	
	strncpy(line->name,       name,       DEFAULT_STR_SIZE);
	strncpy(line->_namespace, _namespace, DEFAULT_STR_SIZE);
	
	line->method       = NULL;
	line->type         = NULL;
	
	line->return_type   = return_type;
	line->cc            = make_cc();
	
	line->dead         = DEAD_UNKNOWN;
	line->halts        = halts;
	
	line->is_private   = false;
	
	line->has_side_effect = has_side_effect;

	return line;
}

struct SSTLine* makeSSTLine2(
	struct Method* m, 
	struct Type* type,
	char* _namespace
){

	struct SSTLine* line = make(SSTLine);
	
	strncpy(line->name,       m->decl->name,    DEFAULT_STR_SIZE);
	strncpy(line->_namespace, _namespace, DEFAULT_STR_SIZE);
	
	line->method       = m;
	line->type         = type;
	
	line->return_type   = m->decl->return_type;
	line->cc            = make_cc();
	
	line->dead         = DEAD_UNKNOWN;
	line->halts        = HALTS_UNKNOWN;
	
	line->is_private   = has_annotation(m->super.annotations, ANNOT_PRIVATE);
	
	line->has_side_effect = m->decl->has_side_effects;

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
		return;
	}

	if(line->type == NULL
	|| line->return_type == NULL
	){
        printf("[SST_PREFILL][Error] line->type or line->return_type not set for %s\n", line->name);
        exit(1);
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
