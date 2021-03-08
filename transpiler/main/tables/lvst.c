#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"
#include "ast/util/str_ast.h"

#include "tables/lvst.h"
#include "tables/symtable.h"

#include "analyzer/lv_analyzer.h"

#include "typeinference/typeinfer.h"
#include "typeinference/util/type_str.h"

#define LVST_INITIAL_CAPACITY 10

struct LVST* makeLVST(){
	
	struct LVST* lvst = malloc(sizeof(struct LVST));
	
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

void lvst_fill(struct Method* subr, struct ST* st, bool debug){
	if(debug){ printf("fillLocalVarSymTable(...)\n"); }
	//fill the local var symbol table	
	//fill lvst with the arguments
	
	if(debug){
		printf("add arguments to LVST\n");
	}
	for(int i = 0; i < subr->count_args; i++){
		
		struct DeclArg* da = subr->args[i];
		
		char* name = da->name; 
		struct Type* type = da->type;
		
		struct LVSTLine* line = malloc(sizeof(struct LVSTLine));
		
		strncpy(line->name, name, DEFAULT_STR_SIZE);
		line->type = type;
		line->isArg = true;
		line->firstOccur = NULL;
		
		lvst_add(st->lvst, line);
	}
	
	//fill lvst with the local variables
	//in the function body
	discoverLVStmtBlock(st, subr->block);
	
	if(debug){
		lvst_print(st->lvst);
	}
}

void freeLVST(struct LVST* lvst){
	
	for(int i = 0; i < lvst->count; i++){
		freeLVSTLine(lvst->lines[i]);
	}
	free(lvst->lines);
	free(lvst);
}

void freeLVSTLine(struct LVSTLine* l){
	
	//the struct AssignStmt* l->firstOccur
	//is freed later on by other means,
	//as it's memory is not managed in the 
	//context of the local variable symbol table
	
	//l->type is also not freed, 
	//for the same reasons
	
	free(l);
}

void lvst_add(struct LVST* lvst, struct LVSTLine* line){
	
	//the local var symbol table works as a set
	//with 'name' as the key
	
	for(int i = 0; i < lvst->count; i++){
		
		struct LVSTLine* current_line = lvst->lines[i];
		
		if(strcmp(current_line->name, line->name) == 0){
			
			//this local var is already present
			//free the line
			freeLVSTLine(line);
			return;
		}
	}
	
	//add the line
	if(lvst->count >= lvst->capacity){
		
		//resize the lvst
		lvst->capacity *= 2;
		
		const int nbytes = 
			sizeof(struct LVSTLine*) * (lvst->capacity);
		
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
	
	printf("%s not found in localvarsymtable\n", name);
	print_exit("Fatal Error");
	return NULL;
}

bool lvst_contains(struct LVST* lvst, char* name){
	
	for(int i = 0; i < lvst->count; i++){
		
		char* lv_name = lvst->lines[i]->name;
		
		if(strcmp(lv_name, name) == 0)
			{ return true; }
	}
	return false;
}

void lvst_print(struct LVST* lvst){
	
	char* fmt = " |% -24s|%-5s| %-24s |\n";
	char* linebig = "------------------------";
	char* line5  = "-----";
	
	printf("Local Variable Symbol Table (LVST)\n");
	printf(fmt, "name", "isArg", "Type");
	printf(fmt, linebig, line5, linebig);
	for(int i = 0; i < lvst->count; i++){
		struct LVSTLine* line = lvst->lines[i];
		
		assert(line != NULL);
		assert(line->type != NULL);
		
		printf(fmt, 
			line->name, 
			(line->isArg)?"yes":"no",
			strType(line->type)
		);
	}
}
