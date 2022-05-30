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

	FILE* header_file;
	FILE *asm_file;
	FILE* file; //where the output will be directed
	uint8_t indent_level;
	
	//unique indexing of the try-statements in a subroutine
	//during transpiling
	uint16_t index_try_stmt; 
	//are we transpiling inside a try-block right now?
	bool in_try_block; 
	
	//the compiler flags
	struct Flags* flags;
	
	bool error;
	
	//symbol tables
	struct ST* tables;
	
	//it is ugly, but this is the name
	//of the current function pointer variable/arg
	//to transpile the subroutine type in 
	//C subroutine signatures
	char current_function_pointer_var_or_arg_name[DEFAULT_STR_SIZE];

};


#endif
