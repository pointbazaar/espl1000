#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "../../../ast/ast.h"
#include "../../../ast/free_ast.h"
#include "localvarsymtable.h"
#include "symtable.h"
#include "../typeinference.h"

// --- declare subroutines private to this compile unit ---
//discover Local Variables
void discoverLVStmtBlock(
	struct ST* st, 
	struct StmtBlock* block,
	bool debug
);

void discoverLVStmt(
	struct ST* st, 
	struct Stmt* stmt,
	bool debug
);

void discoverLVIfStmt(
	struct ST* st, 
	struct IfStmt* i,
	bool debug
);
void discoverLVWhileStmt(
	struct ST* st, 
	struct WhileStmt* w,
	bool debug
);
void discoverLVLoopStmt(
	struct ST* st, 
	struct LoopStmt* l,
	bool debug
);
void discoverLVForStmt(
	struct ST* st, 
	struct ForStmt* l,
	bool debug
);
void discoverLVAssignStmt(
	struct ST* st, 
	struct AssignStmt* a,
	bool debug
);
void discoverLVSwitchStmt(
	struct ST* st, 
	struct SwitchStmt* a,
	bool debug
);
void discoverLVCaseStmt(
	struct ST* st, 
	struct CaseStmt* c, 
	bool debug
);
// --------------------------------------------------------
//to add a row to the local variable symbol table
//the lvst works as a set regarding the 'name' of the local variable
void lvst_add(struct LVST* lvst, struct LVSTLine* line, bool debug);
// --------------------------------------------------------

struct LVST* makeLocalVarSymTable(bool debug){
	
	if(debug){ printf("makeLocalVarSymTable(...)\n"); }
	
	struct LVST* lvst = malloc(sizeof(struct LVST));
	
	lvst->count = 0;
	lvst->capacity = 10;
	lvst->lines = malloc(sizeof(struct LVSTLine*)*lvst->capacity);
	
	return lvst;
}

void fillLocalVarSymTable(
	struct Method* subr, struct ST* st, bool debug
){
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
		
		lvst_add(st->lvst, line, debug);
	}
	
	if(debug){
		printf("add local variables to LVST\n");
	}
	//fill lvst with the local variables
	//in the function body
	discoverLVStmtBlock(st, subr->block, debug);
	
	if(debug){
		lvst_print(st->lvst);
	}
	
	if(debug){
		printf("done constructing LVST\n");
	}
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
	
	//l->type is also not freed, 
	//for the same reasons
	
	free(l);
}

void lvst_add(
	struct LVST* lvst, 
	struct LVSTLine* line,
	bool debug
){
	if(debug){
		printf("lvst_add(%p, %p)\n", lvst, line);
	}
	
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

struct LVSTLine* lvst_get(
	struct LVST* lvst, 
	char* name,
	bool debug
){
	
	if(debug){
		printf("lvst_get(%p, %s)\n", lvst, name);
	}
	
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

void discoverLVStmtBlock(
	struct ST* st, 
	struct StmtBlock* block,
	bool debug
){
	if(debug){ printf("discoverLVStmtBlock\n"); }
	for(int i = 0; i < block->count; i++){
		discoverLVStmt(st, block->stmts[i], debug);
	}
}

void discoverLVStmt(
	struct ST* st, 
	struct Stmt* stmt,
	bool debug
){
	if(debug){ printf("discoverLVStmt\n"); }
	
	switch(stmt->kind){
		
		case 0:
			discoverLVLoopStmt(st, stmt->ptr.m0, debug);
			break;
		case 2:
			discoverLVWhileStmt(st, stmt->ptr.m2, debug);
			break;
		case 3:
			discoverLVIfStmt(st, stmt->ptr.m3, debug);
			break;
		case 5:
			discoverLVAssignStmt(st, stmt->ptr.m5, debug);
			break;
		case 7:
			discoverLVForStmt(st, stmt->ptr.m7, debug);
			break;
		case 8:
			discoverLVSwitchStmt(st, stmt->ptr.m8, debug);
			break;
		default:
			//it is one of the stmt types
			//where there are no declarations inside
			//or simply unconsidered
			break;
	}
}

void discoverLVIfStmt(
	struct ST* st, 
	struct IfStmt* i,
	bool debug
){
	if(debug){ printf("discoverLVIfStmt\n"); }
	
	discoverLVStmtBlock(st, i->block, debug);
	if(i->elseBlock != NULL){
		discoverLVStmtBlock(st, i->elseBlock, debug);
	}
}

void discoverLVWhileStmt(
	struct ST* st, 
	struct WhileStmt* w,
	bool debug
){
	if(debug){ printf("discoverLVWhileStmt\n"); }
	
	discoverLVStmtBlock(st, w->block, debug);
}

void discoverLVLoopStmt(
	struct ST* st, 
	struct LoopStmt* l,
	bool debug
){
	if(debug){ printf("discoverLVLoopStmt\n"); }
	
	discoverLVStmtBlock(st, l->block, debug);
}
void discoverLVForStmt(
	struct ST* st, 
	struct ForStmt* l,
	bool debug
){
	if(debug){ printf("discoverLVForStmt\n"); }
	
	//take the index variable as a local variable
	struct LVSTLine* line = malloc(sizeof(struct LVSTLine));
	
	strncpy(line->name, l->indexName, DEFAULT_STR_SIZE);
	
	line->type = typeFromStr(st, "Int");
	//we have no assignstmt here
	line->firstOccur = NULL; 
	line->isArg = false;;
	
	lvst_add(st->lvst, line, debug);
	
	discoverLVStmtBlock(st, l->block, debug);
}
void discoverLVAssignStmt(
	struct ST* st, 
	struct AssignStmt* a,
	bool debug
){
	if(debug){ printf("discoverLVAssignStmt\n"); }
	
	struct LVSTLine* line = malloc(sizeof(struct LVSTLine));
	
	strncpy(line->name, a->var->simpleVar->name, DEFAULT_STR_SIZE);
	
	//a->optType may be NULL, so be careful here
	if(a->optType != NULL){
		line->type = a->optType;
	}else{
		line->type = inferTypeExpr(st, a->expr, debug);
	}
	
	line->firstOccur = a;
	line->isArg = false;;
	
	lvst_add(st->lvst, line, debug);
}
void discoverLVSwitchStmt(
	struct ST* st, 
	struct SwitchStmt* s,
	bool debug
){
	if(debug){ printf("discoverLVSwitchStmt\n"); }
	
	for(int i=0; i < s->count_cases; i++){
		discoverLVCaseStmt(st, s->cases[i], debug);
	}
}
void discoverLVCaseStmt(
	struct ST* st, 
	struct CaseStmt* c, 
	bool debug
){
	if(debug){ printf("discoverLVCaseStmt\n"); }
	
	if(c->block != NULL){
			discoverLVStmtBlock(st, c->block, debug);
	}
}
//----------------------------------
void lvst_print(struct LVST* lvst){
	//print LVST
	printf("Local Variable Symbol Table (LVST)\n");
	printf("%8s|%8s|%8s\n", "name", "isArg", "Type");
	printf("--------|--------|--------\n");
	for(int i = 0; i < lvst->count; i++){
		struct LVSTLine* line = lvst->lines[i];
		
		assert(line != NULL);
		assert(line->type != NULL);
		
		printf("%8s|%8s|%8s\n", 
			line->name, 
			(line->isArg)?"yes":"no",
			typeToStr(line->type)
		);
	}
}
