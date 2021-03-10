#ifndef LOCALVARSYMTABLE
#define LOCALVARSYMTABLE

#include "ast/ast.h"

struct ST;
struct LVSTLine;

struct LVST {
	//Local Variable Symbol Table (LVST)
	
	unsigned int count;
	size_t capacity;
	
	struct LVSTLine** lines;
};

struct LVSTLine {
	
	char name[DEFAULT_STR_SIZE]; 
	
	struct Type* type; 
	
	//the first occurence
	//in the subroutine
	//where this local variable was 
	//assigned.
	//can be NULL and is not relevant if isArg == true
	struct AssignStmt* firstOccur;
	
	
	//TODO: we must differentiate if 
	//it is an argument to the function
	//or not.
	//because arguments already have a type declared
	//(for the c code generation)
	
	bool isArg;
};

void freeLVSTLine(struct LVSTLine* l);

struct LVST* makeLVST();

void lvst_clear(struct LVST* lvst);

void freeLVST(struct LVST* lvst);

void lvst_add(struct LVST* lvst, struct LVSTLine* line);

struct LVSTLine* lvst_get(struct LVST* lvst, char* name);

bool lvst_contains(struct LVST* lvst, char* name);

void lvst_print(struct LVST* lvst);

#endif
