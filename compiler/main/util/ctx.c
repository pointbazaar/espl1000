
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "compiler/cli/flags/flags.h"
#include "tables/symtable/symtable.h"

#include "ctx.h"

/* this struct represents the context of compilation.
 * it provides information about symbol tables, debug parameters,
 * the output file and such.
 * a pointer to this struct is a passed to each function
 * tasked with compiling an AST node.
 */
struct Ctx {
	//this struct should be opaque outside its implementation file
	//to facilitate encapsulation

	//the compiler flags
	struct Flags* flags;
	
	//symbol tables
	struct ST* tables;
	
	char* asm_filename;
	
};

static char* make_asm_filename(char* filename){

    char* fname_out = malloc(strlen(filename)+4);
    strcpy(fname_out, filename);
    //remove the '.dg'
    fname_out[strlen(fname_out)-3] = '\0';
    strcat(fname_out, ".asm");

    return fname_out;
}

struct Ctx* ctx_ctor(struct Flags* flags, struct ST* tables){

	struct Ctx* res = make(Ctx);
	
	res->flags  = flags;
    res->tables = tables;
    
    res->asm_filename = make_asm_filename(flags_filenames(flags,0));
	
	return res;
}

void ctx_dtor(struct Ctx* ctx){
	
	freeST(ctx_tables(ctx));
    free(ctx_flags(ctx));
    free(ctx->asm_filename);
    free(ctx);
}

struct Flags* ctx_flags(struct Ctx* ctx){
	return ctx->flags;
}


struct ST* ctx_tables(struct Ctx* ctx){
	return ctx->tables;
}

char* ctx_asm_filename(struct Ctx* ctx){
	return ctx->asm_filename;
}
