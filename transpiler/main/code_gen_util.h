#ifndef CODEGENUTIL
#define CODEGENUTIL

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

/* this struct represents the context of transpilation.
 * it provides information about symbol tables, debug parameters,
 * the output file and such.
 * a pointer to this struct is a passed to each subroutine
 * tasked with transpiling an AST node.
 */
struct Ctx {
	
	FILE* file;
	uint8_t indentLevel;
	
	//the transpiler flags 
	struct Flags* flags;
	
	//local variable symbol table 
	// (for the results of local variable type inference)
	struct LVST* lvst;
};

void indent(struct Ctx* ctx);

#endif
