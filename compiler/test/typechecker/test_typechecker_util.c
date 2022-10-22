#include <stdlib.h>
#include <stdio.h>

#include "invoke/invoke.h"
#include "util/fileutils/fileutils.h"
#include "tables/symtable/symtable.h"
#include "util/fill_tables.h"
#include "typechecker/typecheck.h"
#include "ast/util/free_ast.h"
#include "test_typechecker_util.h"

#include "compiler/main/util/ctx.h"
#include "parser/main/util/parser.h"

struct TCError* typecheck_file(char* filename){

    /*
     * build this test suite based on actual .dg files,
     * which would make it easier to create new tests
     * without having to build up the entire AST.
     */

    int status = invoke_lexer(filename);

    if (WEXITSTATUS(status) != 0) {
        printf("[Error] lexer exited with nonzero exit code\n");
        goto end;
    }

    struct Namespace* ns = invoke_parser(filename);

    if (ns == NULL) {
        printf("[Error] parser exited with nonzero exit code\n");
        goto end;
    }
    end:
    ;
    struct AST* ast = build_ast(make_token_filename(filename));

    if(ast == NULL){ return NULL;}

    struct Ctx* ctx = ctx_ctor(makeFlagsSingleFile(filename), makeST(false));

    fill_tables(ast, ctx);

    struct TCError* errors = typecheck_ast(ast, ctx_tables(ctx), false);

    ctx_dtor(ctx);
    
    free_ast(ast);

    return errors;
}
