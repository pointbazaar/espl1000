
#include <stdlib.h>

#include "localvarsymtable.h"

struct LocalVarSymTable* makeLocalVarSymTable(struct Method* subr){
	
	//TODO: implement
	
	return NULL;
}

void freeLVSTLine(struct LVSTLine* l){
	
	//the struct AssignStmt* l->firstOccur
	//is freed later on by other means,
	//as it's memory is not managed in the 
	//context of the local variable symbol table
	
	free(l);
}
