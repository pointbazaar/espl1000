#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include "../util/exit_malloc/exit_malloc.h"
#include "ibuffer.h"
#include "ibuffer_write.h"

#define INSTR_COMMENT_LEN 50
#define INSTR_LABEL_LEN 32

struct Instr {
	//this struct should be opaque outside this file
	enum IKEY key;

	//can be registers, constants, offsets, ...
	//and is interpreted according to 'key'
	int32_t x1;
	int32_t x2;
	int32_t x3;

	//label, etc.
	char str[INSTR_LABEL_LEN];

	//comment for the assembly code
	char comment[INSTR_COMMENT_LEN];
};

struct IBuffer {
	//this struct should be opaque outside this file
	
	struct Instr** buffer;
	uint32_t capacity;
	uint32_t count;
	
};

struct IBuffer* ibu_ctor(){
	
	struct IBuffer* ibu = exit_malloc(sizeof(struct IBuffer));
	
	ibu->capacity = 100;
	ibu->buffer   = exit_malloc(sizeof(struct Instr*)*ibu->capacity);
	ibu->count    = 0;
	
	return ibu;
}

void ibu_dtor(struct IBuffer* ibu){
	
	for(uint32_t k=0;k < ibu->count; k++){
		free(ibu->buffer[k]);
	}
	free(ibu->buffer);
	free(ibu);
}

void ibu_write(struct IBuffer* ibu, FILE* fout){
	
	for(uint32_t k=0;k < ibu->count; k++){
			
		struct Instr* i = ibu->buffer[k];
		
		ibu_write_instr(i->key, i->x1, i->x2, i->x3, i->str, i->comment, fout);
	}
}


void ibu_push(struct IBuffer* ibu, struct Instr* i){
	
	if(ibu->count >= ibu->capacity){
		
		//resize buffer
		ibu->capacity *= 2;
		ibu->buffer = realloc(ibu->buffer, sizeof(struct Instr*) * ibu->capacity);
	}
	
	ibu->buffer[ibu->count] = i;
	ibu->count += 1;
}

void ibu_push4(struct IBuffer* ibu, enum IKEY key, int32_t x1, int32_t x2, int32_t x3, char* label, char* comment){

	struct Instr* i = exit_malloc(sizeof(struct Instr));

	i->key  = key;
	i->x1 = x1;
	i->x2  = x2;
	i->x3 = x3;

	strncpy(i->str, label, INSTR_LABEL_LEN-1);
	i->str[INSTR_LABEL_LEN-1] = '\0';

	strncpy(i->comment, comment, INSTR_COMMENT_LEN-1);
	i->comment[INSTR_COMMENT_LEN-1] = '\0';

	ibu_push(ibu, i);
}
