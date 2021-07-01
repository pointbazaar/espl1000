#ifndef CTX
#define CTX

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

#include "ast/ast.h"

struct ST;

/* this struct represents the context of transpilation.
 * it provides information about symbol tables, debug parameters,
 * the output file and such.
 * a pointer to this struct is a passed to each subroutine
 * tasked with transpiling an AST node.
 */
struct Ctx {
	FILE* c_file;
	FILE* header_file;
	FILE* file; //where the output will be directed
	uint8_t indentLevel;
	
	//the transpiler flags 
	struct Flags* flags;
	
	bool error;
	
	//symbol tables
	struct ST* tables;
	
	//it is ugly, but this is the name
	//of the current function pointer variable/arg
	//to transpile the subroutine type in 
	//C subroutine signatures
	char currentFunctionPointerVarOrArgName[DEFAULT_STR_SIZE];
};


#endif
