#include <stdlib.h>
#include <stdio.h>

#include "invoke/invoke.h"
#include "util/fileutils/fileutils.h"
#include "ast/io/ast_reader.h"
#include "tables/symtable/symtable.h"
#include "util/fill_tables.h"
#include "typechecker/typecheck.h"
#include "ast/util/free_ast.h"
#include "test_typechecker_util.h"

#include "transpiler/main/util/ctx.h"

struct TCError* typecheck_file(char* filename){

    /*
     * build this test suite based on actual .dg files,
     * which would make it easier to create new tests
     * without having to build up the entire AST.
     */

    invoke_lexer_parser(filename);

    char* ast_filenames[1];
    ast_filenames[0] = make_ast_filename(filename);

    struct AST* ast = read_ast(ast_filenames, 1);

    if(ast == NULL){ return NULL;}

    struct Ctx* ctx = malloc(sizeof(struct Ctx));
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

    struct TCError* errors = typecheck_ast(ast, ctx->tables, false);

    freeST(ctx->tables);
    free(ctx);
    free_ast(ast);

    return errors;
}