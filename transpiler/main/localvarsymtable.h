#ifndef LOCALVARSYMTABLE
#define LOCALVARSYMTABLE

#include "../../ast/ast.h"

struct LocalVarSymTable;
struct LVSTLine;

struct LocalVarSymTable {
	//local variable symbol table
	
	unsigned int count;
	
	//name, type, struct AssignStmt* firstOccur
};

struct LVSTLine {
	//a line in the 'local variable symbol table'
	
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
};

void freeLVSTLine(struct LVSTLine* l);

struct LocalVarSymTable* makeLocalVarSymTable(struct Method* subr);

#endif
