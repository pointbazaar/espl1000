#include <stdlib.h>
#include <stdio.h>

#include <invoke/invoke.h>
#include <util/fileutils/fileutils.h>
#include <ast/io/ast_reader.h>
#include <tables/symtable/symtable.h>
#include <util/fill_tables.h>
#include <ast/util/free_ast.h>
#include <assert.h>
#include <analyzer/lv/lv_analyzer.h>
#include "test_typeinference_util.h"

#include "typeinference/typeinfer.h"

#include "transpiler/main/util/ctx.h"

struct Type* typeinfer_in_file(char* filename){

    /*
     * returns the type inferred for the expression in the last return statement
     * of the first function in the file.
     * the last return statement must be the last statement in the function.
     */

    bool success = invoke_lexer_parser(filename);
    assert(success);

    char* ast_filenames[1];
    ast_filenames[0] = make_ast_filename(filename);

    struct AST* ast = read_ast(ast_filenames, 1);
    assert(ast != NULL);

    struct Ctx* ctx     = malloc(sizeof(struct Ctx));
    struct Flags* flags = malloc(sizeof(struct Flags));
    flags->debug = false;

    ctx->tables = makeST(false);

    ctx->error = false;
    ctx->flags = flags;

    ctx->indent_level = 0;
    ctx->file         = NULL;
    ctx->c_file       = NULL;
    ctx->header_file  = NULL;

    ctx->in_try_block   = false;
    ctx->index_try_stmt = 0;

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

    //freeST(ctx->tables);
    //free(ctx);
    //free_ast(ast);

    return returned_type;
}