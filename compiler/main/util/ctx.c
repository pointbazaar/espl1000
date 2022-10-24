#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <malloc.h>
#include <regex.h>

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
};


struct Ctx* ctx_ctor(struct Flags* flags, struct ST* tables){

	struct Ctx* res = make(Ctx);
	
	res->flags  = flags;
    res->tables = tables;
	
	return res;
}

void ctx_dtor(struct Ctx* ctx){
	
	freeST(ctx_tables(ctx));
    freeFlags(ctx_flags(ctx));
    
    free(ctx);
}

struct Flags* ctx_flags(struct Ctx* ctx){
	return ctx->flags;
}


struct ST* ctx_tables(struct Ctx* ctx){
	return ctx->tables;
}


