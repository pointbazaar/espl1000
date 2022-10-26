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
	
	
	//for things like break, continue
	//so they know which label to jump to
	bool is_inside_loop;
	uint32_t label_loop_end;
	uint32_t label_loop_start;
};


struct Ctx* ctx_ctor(struct Flags* flags, struct ST* tables){

	struct Ctx* res = make(Ctx);
	
	res->flags  = flags;
    res->tables = tables;
    
    res->is_inside_loop = false;
	
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

void ctx_enter_loop(struct Ctx* ctx, uint32_t label_start, uint32_t label_end){
	ctx->label_loop_start = label_start;
	ctx->label_loop_end   = label_end;
	ctx->is_inside_loop   = true;
}

void ctx_exit_loop(struct Ctx* ctx){
	if(!ctx->is_inside_loop){
		printf("fatal error in ctx_exit_loop. Was not in a loop. Exiting");
		fflush(stdout);
		exit(1);
	}
	ctx->is_inside_loop = false;
}

uint32_t ctx_get_label_loop_start(struct Ctx* ctx){
	
	if(!ctx->is_inside_loop){
		printf("fatal error in ctx_get_label_loop_start. Was not in a loop. Exiting");
		fflush(stdout);
		exit(1);
	}
	return ctx->label_loop_start;
}

uint32_t ctx_get_label_loop_end(struct Ctx* ctx){
	
	if(!ctx->is_inside_loop){
		printf("fatal error in ctx_get_label_loop_end. Was not in a loop. Exiting");
		fflush(stdout);
		exit(1);
	}
	return ctx->label_loop_end;
}


