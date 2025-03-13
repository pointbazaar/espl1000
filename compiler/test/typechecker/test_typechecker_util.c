#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "invoke/invoke.h"
#include "util/fileutils/fileutils.h"
#include "tables/symtable/symtable.h"
#include "util/fill_tables.h"
#include "typechecker/typecheck.h"
#include "ast/util/free_ast.h"
#include "test_typechecker_util.h"

#include "compiler/main/util/ctx.h"
#include "parser/main/util/parser.h"

struct TCError* typecheck_file(char* filename) {

	/*
	 * build this test suite based on actual .dg files,
	 * which would make it easier to create new tests
	 * without having to build up the entire AST.
	 */

	int fd = invoke_lexer(filename, false);

	if (fd < 0) {
		printf("[Error] lexer exited with nonzero exit code\n");
		fflush(stdout);
		return NULL;
	}

	struct Ctx* ctx = ctx_ctor(makeFlagsSingleFile(filename), st_ctor(false));

	struct AST* ast = build_ast(fd, flags_token_filename(ctx_flags(ctx)));

	if (ast == NULL) { return NULL; }

	fill_tables(ast, ctx);

	struct TCError* errors = typecheck_ast(ast, ctx, false);

	ctx_dtor(ctx);

	free_ast(ast);

	close(fd);

	return errors;
}
