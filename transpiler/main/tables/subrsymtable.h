#ifndef SUBRSYMTABLE
#define SUBRSYMTABLE

#include <stdbool.h>

#include "../../../ast/ast.h"

struct SSTLine {
	//Subroutine Symbol Table Line
	
	//the name of the subroutine
	char name[DEFAULT_STR_SIZE]; 
	
	//a reference to the type being returned
	struct Type* returnType; 
	
	//if this function
	//comes from the C Standard Library
	bool isLibC;
};

struct SST {
	//Subroutine Symbol Table
	
	unsigned int count;
	size_t capacity;
	
	struct SSTLine** lines;
};



// -----------------------


struct SST* makeSubrSymTable(struct Namespace* ns, bool debug);

void freeSubrSymTable(struct SST* sst);

void freeSSTLine(struct SSTLine* l);

void sst_add(struct SST* sst, struct SSTLine* line);

struct SSTLine* sst_get(struct SST* sst, char* name);

#endif
