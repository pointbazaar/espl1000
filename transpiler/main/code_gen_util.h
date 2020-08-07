#ifndef CODEGENUTIL
#define CODEGENUTIL

#include <stdio.h>
#include <stdbool.h>

/* this struct represents the context of transpilation.
 * it provides information about symbol tables, debug parameters,
 * the output file and such.
 * a pointer to this struct is a passed to each subroutine
 * tasked with transpiling an AST node.
 */
struct Ctx {
	
	FILE* file;
	bool debug;
	int indentLevel;
};

void indent(struct Ctx* ctx);

#endif
