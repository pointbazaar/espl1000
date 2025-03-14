#include <stdlib.h>
#include <stdio.h>

#include <invoke/invoke.h>
#include <unistd.h>
#include <util/fileutils/fileutils.h>
#include <tables/symtable/symtable.h>
#include <util/fill_tables.h>
#include <ast/util/free_ast.h>
#include <assert.h>
#include <analyzer/lv/lv_analyzer.h>
#include <ast/util/copy_ast.h>
#include "test_typeinference_util.h"

#include "typeinference/typeinfer.h"

#include "compiler/main/util/ctx.h"
#include "parser/main/util/parser.h"

#include "lexer/src/lexer_flags.h"
#include "lexer/src/lexer_main.h"

struct Type* typeinfer_in_file(char* filename) {

	/*
	 * returns the type inferred for the expression in the last return statement
	 * of the first function in the file.
	 * the last return statement must be the last statement in the function.
	 */

	bool s = true;

	struct LexerFlags flags;
	flags.write_token_file = false;
	flags.filename = filename;
	flags.debug = false;

	struct TokenList* list = lexer_main(&flags);

	if (!list) {
		fprintf(stderr, "[Error] lexer exited with nonzero exit code\n");
		s = false;
		return NULL;
	}

	bool success = s;
	assert(success);

	struct Ctx* ctx = ctx_ctor(makeFlagsSingleFile(filename), st_ctor(false));

	struct AST* ast = build_ast(list, flags_token_filename(ctx_flags(ctx)));

	assert(ast != NULL);

	assert(ast->count_namespaces >= 1);

	fill_tables(ast, ctx);

	struct Namespace* ns = ast->namespaces[0];
	assert(ns != NULL);
	assert(ns->name != NULL);
	assert(ns->count_methods >= 1);

	struct Method* m = ns->methods[0];
	assert(m != NULL);
	assert(m->decl->name != NULL);
	assert(m->block->count >= 1);

	lvst_fill(m, ctx_tables(ctx));

	struct Stmt* stmt = m->block->stmts[m->block->count - 1];
	assert(stmt != NULL);
	assert(stmt->kind == 4);

	struct Expr* expr = stmt->ptr.m4->return_value;
	struct Type* returned_type = infer_type_expr(ctx_tables(ctx), expr);

	struct Type* copy = copy_type(returned_type);

	ctx_dtor(ctx);

	free_ast(ast);

	freeTokenList(list);

	return copy;
}
