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
#include "cg_x86_single_function.h"
#include "cg_x86_single_tac.h"

static void visitor_emit_extern(void* node, enum NODE_TYPE type, void* arg) {

	if (type != NODE_METHOD) {
		return;
	}

	FILE* fout = (FILE*)arg;

	struct Method* m = (struct Method*)node;

	if (has_annotation(m->super.annotations, ANNOT_EXTERN)) {
		fprintf(fout, "extern %s\n", m->decl->name);
	}
}

static void declare_extern_functions_asm(FILE* fout, struct AST* ast) {

	visit_ast(ast, visitor_emit_extern, fout);
}

bool compile_and_write_x86(struct AST* ast, struct Ctx* ctx) {

	struct IBuffer* ibu = ibu_ctor();

	char* c = "call main";

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

	//convert AST into 3 address code with temporaries, use recursive descent to make TAC
	for (size_t i = 0; i < ast->count_namespaces; i++) {
		struct Namespace* ns = ast->namespaces[i];

		for (size_t j = 0; j < ns->count_methods; j++) {
			struct Method* m = ns->methods[j];

			compile_and_write_x86_single_function(m, ctx, ibu);
		}
	}

	FILE* fout = fopen(flags_asm_filename(ctx_flags(ctx)), "w");
	if (fout == NULL) {
		printf("error opening output file\n");
		ibu_dtor(ibu);
		return false;
	}

	fprintf(fout, "section .text\n");
	fprintf(fout, "global _start\n\n");

	declare_extern_functions_asm(fout, ast);

	ibu_write(ibu, fout);

	fclose(fout);

	ibu_dtor(ibu);

	return true;
}
