#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "util/fileutils/fileutils.h"
#include "tables/symtable/symtable.h"
#include "util/fill_tables.h"
#include "typechecker/typecheck.h"
#include "ast/util/free_ast.h"
#include "test_typechecker_util.h"

#include "compiler/main/util/ctx.h"
#include "parser/main/util/parser.h"
#include "lexer/src/lexer_flags.h"
#include "lexer/src/lexer_main.h"

struct TCError* typecheck_file(char* filename) {

	/*
	 * build this test suite based on actual .dg files,
	 * which would make it easier to create new tests
	 * without having to build up the entire AST.
	 */

	struct LexerFlags flags;
	flags.write_token_file = false;
	flags.filename = filename;
	flags.debug = false;

	struct TokenList* list = lexer_main(&flags);

	if (!list) {
		printf("[Error] lexer exited with nonzero exit code\n");
		fflush(stdout);
		return NULL;
	}

	struct Ctx* ctx = ctx_ctor(makeFlagsSingleFile(filename), st_ctor(false));

	struct AST* ast = build_ast(list, flags_token_filename(ctx_flags(ctx)));

	if (ast == NULL) { return NULL; }

	fill_tables(ast, ctx);

	struct TCError* errors = typecheck_ast(ast, ctx, false);

	ctx_dtor(ctx);

	free_ast(ast);

	freeTokenList(list);

	return errors;
}
