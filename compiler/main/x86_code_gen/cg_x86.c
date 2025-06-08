#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tables/symtable/symtable.h"
#include "rat/rat.h"

#include "ast/ast.h"
#include "ast/visitor/visitor.h"

#include "cli/flags/flags.h"

#include "util/ctx.h"

#include "ibuffer/ibuffer_x86.h"

#include "cg_x86.h"
#include "tac/tacbuffer.h"
#include "tac/tac.h"
#include "cg_x86_single_tac.h"

static void visitor_emit_extern(void* node, enum NODE_TYPE type, void* arg) {

	if (type != NODE_METHOD) {
		return;
	}

	struct IBuffer* ibu = (struct IBuffer*)arg;

	struct Method* m = (struct Method*)node;

	if (has_annotation(m->super.annotations, ANNOT_EXTERN)) {
		nasm_extern(m->decl->name);
	}
}

static void declare_extern_functions_asm(struct IBuffer* ibu, struct AST* ast) {

	visit_ast(ast, visitor_emit_extern, ibu);
}

bool prologue_x86(struct Ctx* ctx, struct IBuffer* ibu, struct AST* ast) {

	if (!data_write_data_segment(ctx_tables(ctx)->data, ibu)) {
		return false;
	}

	declare_extern_functions_asm(ibu, ast);

	char* c = "call main";

	section(".text");
	global("_start");

	label("_start");

	call("main", c);

	// exit code was returned in RAX by 'main' function
	mov_regs(SD_REG_RDI, SD_REG_RAX, c);
	// syscall number
	mov_const(SD_REG_RAX, 60, c);

	// make syscall
	x86_syscall(c);

	// few nops so the debugger
	// has something to step through
	nop(c);
	nop(c);
	nop(c);

	return true;
}
