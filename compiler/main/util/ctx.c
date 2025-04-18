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
#include "cli/flags/flags.h"
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

	uint32_t loop_depth; //how many loops are we inside

	uint32_t label_loop_end; //label of current loop start
	uint32_t label_loop_start; //label of current loop end
};

struct Ctx* ctx_ctor(struct Flags* flags, struct ST* tables) {

	struct Ctx* res = make(Ctx);

	res->flags = flags;
	res->tables = tables;

	res->loop_depth = 0;

	return res;
}

void ctx_dtor(struct Ctx* ctx) {

	st_free(ctx_tables(ctx));
	freeFlags(ctx_flags(ctx));

	free(ctx);
}

struct Flags* ctx_flags(struct Ctx* ctx) {
	return ctx->flags;
}

struct ST* ctx_tables(struct Ctx* ctx) {
	return ctx->tables;
}

void ctx_enter_loop(struct Ctx* ctx, uint32_t label_start, uint32_t label_end) {
	ctx->label_loop_start = label_start;
	ctx->label_loop_end = label_end;
	ctx->loop_depth++;
}

bool ctx_exit_loop(struct Ctx* ctx) {
	if (ctx->loop_depth == 0) {
		fprintf(stderr, "fatal error in %s. Was not in a loop.", __func__);
		return false;
	}
	ctx->loop_depth--;

	return true;
}

int32_t ctx_get_label_loop_start(struct Ctx* ctx) {

	if (ctx->loop_depth == 0) {
		fprintf(stderr, "fatal error in %s. Was not in a loop.", __func__);
		return -1;
	}
	return ctx->label_loop_start;
}

int32_t ctx_get_label_loop_end(struct Ctx* ctx) {

	if (ctx->loop_depth == 0) {
		fprintf(stderr, "fatal error in %s. Was not in a loop", __func__);
		return -1;
	}
	return ctx->label_loop_end;
}
