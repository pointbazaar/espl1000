#include <stdlib.h>
#include <stdio.h>

#include <invoke/invoke.h>
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

struct Type* typeinfer_in_file(char* filename){

    /*
     * returns the type inferred for the expression in the last return statement
     * of the first function in the file.
     * the last return statement must be the last statement in the function.
     */

    bool s = true;
    int status = invoke_lexer(filename);

    if (WEXITSTATUS(status) != 0) {
        printf("[Error] lexer exited with nonzero exit code\n");
        s = false;
        goto end;
    }

    end:;
    bool success = s;
    assert(success);

    struct AST* ast = build_ast(make_token_filename(filename));

    assert(ast != NULL);

    struct Ctx* ctx     = malloc(sizeof(struct Ctx));
    struct Flags* flags = malloc(sizeof(struct Flags));
    flags->debug = false;

    ctx->tables = makeST(false);

    ctx->error = false;
    ctx->flags = flags;

    ctx->indent_level = 0;
    ctx->file         = NULL;

    fill_tables(ast, ctx);

    struct Namespace* ns = ast->namespaces[0];
    assert(ns != NULL);
    assert(ns->count_methods >= 1);

    struct Method* m = ns->methods[0];
    assert(m != NULL);
    assert(m->block->count >= 1);

    lvst_fill(m, ctx->tables);

    struct Stmt* stmt = m->block->stmts[m->block->count-1];
    assert(stmt != NULL);
    assert(stmt->kind == 4);

    struct Expr* expr = stmt->ptr.m4->return_value;
    struct Type* returned_type = infer_type_expr(ctx->tables, expr);

	struct Type* copy = copy_type(returned_type);

	freeST(ctx->tables);
	free(ctx);
	free_ast(ast);

	free(flags);

    return copy;
}