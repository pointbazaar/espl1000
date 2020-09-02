#ifndef STRUCTSYMTABLE
#define STRUCTSYMTABLE

//to include DEFAULT_STR_SIZE
#include "../../../ast/ast.h"

//TODO: finish the implementation 

struct STSTLine {
	
	char name[DEFAULT_STR_SIZE];
	
};

struct STST {
	
	//STruct Symbol Table
	
	struct STSTLine** lines;
	
	unsigned int count;
	unsigned int capacity;
};

struct STST* makeStructSymTable(struct Namespace* ns, bool debug);

void freeStructSymTable(struct STST* stst);

#endif
