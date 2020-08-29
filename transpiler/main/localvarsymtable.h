#ifndef LOCALVARSYMTABLE
#define LOCALVARSYMTABLE

#include "../../ast/ast.h"

struct LVST;
struct LVSTLine;

struct LVST {
	//Local Variable Symbol Table (LVST)
	//local variable symbol table
	
	unsigned int count;
	size_t capacity;
	
	struct LVSTLine** lines;
};

struct LVSTLine {
	//a line in the 'local variable symbol table'
	//name, type, struct AssignStmt* firstOccur
	
	//max 32 chars for a local variable identifier
	char name[32]; 
	
	//the literal name of the type, 
	//e.g. 'PInt', 'MyStruct'
	char type[32]; 
	
	//the first occurence
	//in the subroutine
	//where this local variable was 
	//assigned
	struct AssignStmt* firstOccur;
	
	
	//TODO: we must differentiate if 
	//it is an argument to the function
	//or not.
	//because arguments already have a type declared
	//(for the c code generation)
	
	
	//is it an argument
	bool isArg;
};

void freeLVSTLine(struct LVSTLine* l);

struct LVST* makeLocalVarSymTable(struct Method* subr);

//to add a row to the local variable symbol table
//the lvst works as a set regarding the 'name' of the local variable
void lvst_add(struct LVST* lvst, struct LVSTLine* line);

//obtain a reference to the line for that identifier
struct LVSTLine* lvst_get(struct LVST* lvst, char* name);

#endif
