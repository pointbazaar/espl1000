#ifndef LOCALVARSYMTABLE
#define LOCALVARSYMTABLE

#include "../../ast/ast.h"

struct SST;
struct SSTLine;

struct SST {
	//Subroutine Symbol Table
	
	unsigned int count;
	size_t capacity;
	
	struct SSTLine** lines;
};

struct SSTLine {
	//Subroutine Symbol Table Line
	
	//the name of the subroutine
	char name[32]; 
	
	//the literal name of the type, 
	//e.g. 'PInt', 'MyStruct'
	char returnType[32]; 
	
	//if this function
	//comes from the C Standard Library
	bool isLibC;
};

#endif
