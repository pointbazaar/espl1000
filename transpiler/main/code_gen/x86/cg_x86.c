#include <stdio.h>
#include <stdbool.h>
#include <tables/symtable/symtable.h>
#include <stdlib.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "flags.h"
#include "util/ctx.h"

#include "cg_x86.h"

static void compile_ast_x86(struct AST* ast, struct Ctx* ctx);
static void compile_namespace_x86(struct Namespace* ns, struct Ctx* ctx);
static void compile_method_x86(struct Method* m, struct Ctx* ctx);
static void compile_stmtblock_x86(struct StmtBlock* block, struct Ctx* ctx);
static void compile_stmt_x86(struct Stmt* stmt, struct Ctx* ctx);
/*
static void compile_call_x86(struct Call* call, struct Ctx* ctx);
static void compile_while_stmt_x86(struct WhileStmt* m, struct Ctx* ctx);
static void compile_if_stmt_x86(struct IfStmt* m, struct Ctx* ctx);
 */

bool compile_and_write_x86(char* asm_file_filename, struct AST* ast, struct Flags* flags){

    struct Ctx* ctx = make(Ctx);

    ctx->tables = makeST(flags->debug);

    ctx->error = false;
    ctx->flags = flags;

    ctx->indent_level = 0;
    ctx->file         = NULL;
    ctx->c_file       = NULL;
    ctx->header_file  = NULL;

    ctx->in_try_block   = false;
    ctx->index_try_stmt = 0;

    ctx->asm_file      = fopen(asm_file_filename, "w");
    //full buffering for performance
    setvbuf(ctx->asm_file, NULL, _IOFBF, BUFSIZ);

    compile_ast_x86(ast, ctx);

    fclose(ctx->asm_file);

    freeST(ctx->tables);

    const bool status = !(ctx->error);

    free(ctx);

    return status;
}

static void compile_ast_x86(struct AST* ast, struct Ctx* ctx){

    for(uint32_t i = 0; i < ast->count_namespaces; i++){
        compile_namespace_x86(ast->namespaces[i], ctx);
    }
}

static void compile_namespace_x86(struct Namespace* ns, struct Ctx* ctx){

    for(uint32_t i = 0; i < ns->count_methods; i++){
        compile_method_x86(ns->methods[i], ctx);
    }
}

static void compile_method_x86(struct Method* m, struct Ctx* ctx){

    //TODO: compile label for method,
    //TODO: cdecl calling convention stuff, ...

    compile_stmtblock_x86(m->block, ctx);
}

static void compile_stmtblock_x86(struct StmtBlock* block, struct Ctx* ctx){

    for(uint32_t i = 0; i < block->count; i++){
        compile_stmt_x86(block->stmts[i], ctx);
    }
}

static void compile_stmt_x86(struct Stmt* stmt, struct Ctx* ctx){

    printf("%d\n", ctx->indent_level);

    //TODO
    switch (stmt->kind) {
        /*
        case 1: compile_call_x86(stmt->ptr.m1, ctx); break;
        case 2: compile_while_stmt_x86(stmt->ptr.m2, ctx); break;
        case 3: compile_if_stmt_x86(stmt->ptr.m3, ctx); break;
         */
        default:
            puts("[Error] not implemented");
    }
}
/*
static void compile_call_x86(struct Call* call, struct Ctx* ctx){

    call->args;
    //TODO
    puts("[Error] not implemented");
}

static void compile_while_stmt_x86(struct WhileStmt* m, struct Ctx* ctx){

    m->condition;
    //TODO
    puts("[Error] not implemented");
}

static void compile_if_stmt_x86(struct IfStmt* m, struct Ctx* ctx){

    m->block;
    //TODO
    puts("[Error] not implemented");
}
 */