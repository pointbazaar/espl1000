#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../../ast/ast.h"
#include "../../../util/util.h"

#include "localvarsymtable.h"

// --- declare subroutines private to this compile unit ---
//discover Local Variables
void discoverLVStmtBlock(struct LVST* lvst, struct StmtBlock* block);

void discoverLVStmt(struct LVST* lvst, struct Stmt* stmt);

void discoverLVIfStmt(struct LVST* lvst, struct IfStmt* i);
void discoverLVWhileStmt(struct LVST* lvst, struct WhileStmt* w);
void discoverLVLoopStmt(struct LVST* lvst, struct LoopStmt* l);
void discoverLVAssignStmt(struct LVST* lvst, struct AssignStmt* a);
// --------------------------------------------------------
//to add a row to the local variable symbol table
//the lvst works as a set regarding the 'name' of the local variable
void lvst_add(struct LVST* lvst, struct LVSTLine* line);
// --------------------------------------------------------

struct LVST* makeLocalVarSymTable(struct Method* subr, bool debug){
	
	if(debug){ printf("makeLocalVarSymTable(...)\n"); }
	
	struct LVST* lvst = smalloc(sizeof(struct LVST));
	
	lvst->count = 0;
	lvst->capacity = 10;
	lvst->lines = smalloc(sizeof(struct LVSTLine*)*lvst->capacity);
	
	//fill the local var symbol table
	
	//fill lvst with the arguments
	
	if(debug){
		printf("add arguments to LVST\n");
	}
	for(int i = 0; i < subr->count_args; i++){
		
		struct DeclArg* da = subr->args[i];
		
		char* name = da->name; 
		struct Type* type = da->type;
		
		struct LVSTLine* line = smalloc(sizeof(struct LVSTLine));
		
		strncpy(line->name, name, DEFAULT_STR_SIZE);
		line->type = type;
		line->isArg = true;
		line->firstOccur = NULL;
		
		lvst_add(lvst, line);
	}
	
	if(debug){
		printf("add local variables to LVST\n");
	}
	//fill lvst with the local variables
	//in the function body
	discoverLVStmtBlock(lvst, subr->block);
	
	if(debug){
		//print LVST
		
		printf("Local Variable Symbol Table (LVST)\n");
		printf("%8s|%8s\n", "name", "isArg");
		printf("--------|--------\n");
		for(int i = 0; i < lvst->count; i++){
			struct LVSTLine* line = lvst->lines[i];
			
			printf("%8s|%8s\n", line->name, (line->isArg)?"yes":"no");
		}
		
	}
	
	if(debug){
		printf("done constructing LVST\n");
	}
	
	return lvst;
}

void freeLocalVarSymTable(struct LVST* lvst){
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
	
	//l->type is also not freed, for the same reasons
	
	free(l);
}

void lvst_add(struct LVST* lvst, struct LVSTLine* line){
	
	//no debug param, so we print
	printf("lvst_add(...)\n");
	
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
	
	//no debug param, so we print
	printf("lvst_get(..., %s)\n", name);
	
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

// --------------------------------------------------------

void discoverLVStmtBlock(struct LVST* lvst, struct StmtBlock* block){
	printf("discoverLVStmtBlock\n");
	for(int i = 0; i < block->count; i++){
		discoverLVStmt(lvst, block->stmts[i]);
	}
}
void discoverLVStmt(struct LVST* lvst, struct Stmt* stmt){
	printf("discoverLVStmt\n");
	if(stmt->m0 != NULL){
		discoverLVLoopStmt(lvst, stmt->m0);
	}else if(stmt->m2 != NULL){
		discoverLVWhileStmt(lvst, stmt->m2);
	}else if(stmt->m3 != NULL){
		discoverLVIfStmt(lvst, stmt->m3);
	}else if(stmt->m5 != NULL){
		discoverLVAssignStmt(lvst, stmt->m5);
	}
}
void discoverLVIfStmt(struct LVST* lvst, struct IfStmt* i){
	printf("discoverLVIfStmt\n");
	discoverLVStmtBlock(lvst, i->block);
	if(i->elseBlock != NULL){
		discoverLVStmtBlock(lvst, i->elseBlock);
	}
}
void discoverLVWhileStmt(struct LVST* lvst, struct WhileStmt* w){
	printf("discoverLVWhileStmt\n");
	discoverLVStmtBlock(lvst, w->block);
}
void discoverLVLoopStmt(struct LVST* lvst, struct LoopStmt* l){
	printf("discoverLVLoopStmt\n");
	discoverLVStmtBlock(lvst, l->block);
}
void discoverLVAssignStmt(struct LVST* lvst, struct AssignStmt* a){
	printf("discoverLVAssignStmt\n");
	struct LVSTLine* line = smalloc(sizeof(struct LVSTLine));
	
	strncpy(line->name, a->var->simpleVar->name, DEFAULT_STR_SIZE);
	
	//a->optType may be NULL, so be careful here
	line->type = a->optType;
	
	line->firstOccur = a;
	line->isArg = false;;
	
	lvst_add(lvst, line);
}
