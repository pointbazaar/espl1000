#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast/ast.h"

#include "token/TokenKeys.h"

#include "stst.h"
#include "stst_print.h"

#define STST_INITIAL_CAPACITY 10;

#define ERR_NOT_FOUND "[STST] could not find struct \n"

struct STST {
	//STruct Symbol Table (STST)
	
	//struct STST should be opaque,
	//except in this file, for better encapsulation
	
	struct STSTLine** lines;
	
	unsigned int count;
	unsigned int capacity;
};

struct STST* makeSTST(){
	
	struct STST* stst = make(STST);
	
	stst->capacity = STST_INITIAL_CAPACITY;
	stst->lines    = malloc(sizeof(struct STSTLine*) * stst->capacity);
	stst->count    = 0;
	
	return stst;
}

void stst_fill(struct STST* stst, struct Namespace* ns){

	for(int i=0;i < ns->count_structs; i++){

		struct StructDecl* mystruct = ns->structs[i];

		struct STSTLine* line = makeSTSTLine(mystruct, ns->name);

		stst_add(stst, line);
	}
}

struct STSTLine* stst_get(struct STST* stst, char* name){
	
	for(int i = 0; i < stst->count; i++){
		
		struct STSTLine* line = stst->lines[i];
		
		if(strncmp(line->name, name, DEFAULT_STR_SIZE-1) == 0)
			{ return line; }
	}
	
	printf(ERR_NOT_FOUND);
	printf("\t%s\n", name);
	exit(1);
	return NULL;
}

struct StructMember* stst_get_member(struct STST* stst, char* struct_name, char* member_name){
	
	struct STSTLine* line = stst_get(stst, struct_name);

	for(int j=0; j < line->decl->count_members; j++){

		struct StructMember* member = line->decl->members[j];

		if(strcmp(member->name, member_name) == 0){ return member; }
	}

	char* msg = "[STST] could not find member '%s' of '%s'\n";
	printf(msg, member_name, struct_name);
	
	stst_print(stst);
	exit(1);
}

uint32_t stst_member_offset(struct STST* stst, char* struct_name, char* member_name){
	
	//calculates the offset in bytes, from the start of the struct
	
	struct STSTLine* line = stst_get(stst, struct_name);
	
	uint32_t offset = 0;

	for(int j=0; j < line->decl->count_members; j++){

		struct StructMember* member = line->decl->members[j];
		
		struct Type* t = member->type;
		uint32_t size = 0;
		
		if(t->basic_type != NULL){
			if(t->basic_type->simple_type != NULL){
				struct SimpleType* st = t->basic_type->simple_type;
				
				if(st->primitive_type != NULL){
					size = 1;
				}else{
					//is, structtype, pointer, 2 bytes
					size = 2;
				}
			}else{
				//subrtype, pointer, 2 bytes
				size = 2;
			}
		}else if(t->array_type != NULL){
			//pointer, therefore 2 bytes
			size = 2;
		}else{
			//TODO
		}

		if(strcmp(member->name, member_name) == 0){ return offset; }
		
		offset += size;
	}
	
	printf("[STST] could not find member offset, struct '%s', member '%s'\n", struct_name, member_name);
	fflush(stdout);
	exit(1);
	
	return 0;
}

uint32_t stst_size(struct STST* stst){
	return stst->count;
}

struct STSTLine* stst_at(struct STST* stst, uint32_t index){
	return stst->lines[index];
}

void freeSTST(struct STST* stst){
	
	for(int i=0;i < stst->count; i++){
		free(stst->lines[i]);
	}
	
	free(stst->lines);
	
	free(stst);
}

struct STSTLine* makeSTSTLine(struct StructDecl* s, char* _namespace){
	
	struct STSTLine* line = make(STSTLine);

	line->decl       = s;
	line->is_private = has_annotation(s->super.annotations, ANNOT_PRIVATE);
	
	strncpy(line->_namespace, _namespace,                    DEFAULT_STR_SIZE);
	strncpy(line->name, s->type->struct_type->type_name, DEFAULT_STR_SIZE);
	
	return line;
}

void stst_add(struct STST* stst, struct STSTLine* line){
	
	if(stst->capacity <= stst->count){
		
		stst->capacity *= 2;
		stst->lines = realloc(stst->lines, sizeof(struct STSTLine*) * stst->capacity);
	}

	stst->lines[stst->count] = line;
	stst->count += 1;
}
