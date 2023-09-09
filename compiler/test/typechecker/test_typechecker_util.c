#include <stdio.h>
#include <stdlib.h>

#include "ast/util/free_ast.h"
#include "compiler/main/util/ctx.h"
#include "invoke/invoke.h"
#include "parser/main/util/parser.h"
#include "tables/symtable/symtable.h"
#include "test_typechecker_util.h"
#include "typechecker/typecheck.h"
#include "util/fileutils/fileutils.h"
#include "util/fill_tables.h"

struct TCError* typecheck_file(char* filename) {

	/*
	 * build this test suite based on actual .dg files,
	 * which would make it easier to create new tests
	 * without having to build up the entire AST.
	 */

	int status = invoke_lexer(filename);

	if(WEXITSTATUS(status) != 0) {
		printf("[Error] lexer exited with nonzero exit code\n");
		fflush(stdout);
		exit(1);
	}

	struct Ctx* ctx = ctx_ctor(makeFlagsSingleFile(filename), makeST(false));

	struct AST* ast = build_ast(flags_token_filename(ctx_flags(ctx)));

	if(ast == NULL) {
		return NULL;
	}

	fill_tables(ast, ctx);

	struct TCError* errors = typecheck_ast(ast, ctx_tables(ctx), false);

	ctx_dtor(ctx);

	free_ast(ast);

	return errors;
}
