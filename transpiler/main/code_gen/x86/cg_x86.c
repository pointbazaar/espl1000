#include <stdio.h>
#include <stdbool.h>
#include <tables/symtable/symtable.h>
#include <stdlib.h>

#include "ast/ast_declare.h"
#include "ast/ast.h"

#include "flags.h"
#include "util/ctx.h"

#include "cg_x86.h"
#include "_emit_x86.h"

static void compile_ast_x86(struct AST* ast, struct Ctx* ctx);
static void compile_namespace_x86(struct Namespace* ns, struct Ctx* ctx);
static void compile_method_x86(struct Method* m, struct Ctx* ctx);
static void compile_stmtblock_x86(struct StmtBlock* block, struct Ctx* ctx);
static void compile_stmt_x86(struct Stmt* stmt, struct Ctx* ctx);
static void compile_call_x86(struct Call* call, struct Ctx* ctx);
//static void compile_while_stmt_x86(struct WhileStmt* m, struct Ctx* ctx);
//static void compile_if_stmt_x86(struct IfStmt* m, struct Ctx* ctx);
static void compile_ret_stmt_x86(struct RetStmt* m, struct Ctx* ctx);
//static void compile_assign_stmt_x86(struct AssignStmt* a, struct Ctx* ctx);
//static void compile_try_catch_stmt_x86(struct TryCatchStmt* tcs, struct Ctx* ctx);
//static void compile_for_stmt_x86(struct ForStmt* m, struct Ctx* ctx);
//static void compile_switch_stmt_x86(struct SwitchStmt* m, struct Ctx* ctx);
static void compile_expr_x86(struct Expr* expr, struct Ctx* ctx);
static void compile_term_x86(struct Term* term, struct Ctx* ctx);
static void compile_variable_x86(struct Variable* var, struct Ctx* ctx);
static void compile_unopterm_x86(struct UnOpTerm* uop, struct Ctx* ctx);
static void compile_simplevar_x86(struct SimpleVar* s, struct Ctx* ctx);

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

    switch (stmt->kind) {
        case 1: compile_call_x86(stmt->ptr.m1, ctx); break;
        //case 2: compile_while_stmt_x86(stmt->ptr.m2, ctx); break;
        //case 3: compile_if_stmt_x86(stmt->ptr.m3, ctx); break;
        case 4: compile_ret_stmt_x86(stmt->ptr.m4, ctx); break;
        //case 5: compile_assign_stmt_x86(stmt->ptr.m5, ctx); break;
        //case 7: compile_for_stmt_x86(stmt->ptr.m7, ctx); break;
        //case 8: compile_switch_stmt_x86(stmt->ptr.m8, ctx); break;
        default:
            if(stmt->is_break){
                //TODO
                puts("[Error] not implemented");
            }else if(stmt->is_continue){
                //TODO
                puts("[Error] not implemented");
            }
    }
}

static void compile_call_x86(struct Call* call, struct Ctx* ctx){

    if(call->callable->member_access == NULL && call->callable->simple_var->count_indices == 0){

        //TODO: put the arguments in the appropriate registers / on the stack
        puts("[Error] not implemented");

        emit2("call", call->callable->simple_var->name, ctx->asm_file);
    }else{
        //TODO
        puts("[Error] not implemented");
    }
}

/*
static void compile_while_stmt_x86(struct WhileStmt* m, struct Ctx* ctx){

    m->condition;
    //TODO
    puts("[Error] not implemented");
}
 */
/*
static void compile_if_stmt_x86(struct IfStmt* m, struct Ctx* ctx){

    m->block;
    //TODO
    puts("[Error] not implemented");
}
 */

static void compile_ret_stmt_x86(struct RetStmt* m, struct Ctx* ctx){

    //TODO: follow cdecl calling convention
    //generate code which puts the expression value on the stack,
    //then put that into rax

    compile_expr_x86(m->return_value, ctx);

    emit2("pop", "rax", ctx->asm_file);
    emit1("ret", ctx->asm_file);
}

/*
static void compile_assign_stmt_x86(struct AssignStmt* a, struct Ctx* ctx){
    //TODO
    puts("[Error] not implemented");
}
 */
/*
static void compile_try_catch_stmt_x86(struct TryCatchStmt* tcs, struct Ctx* ctx){
    //TODO
    puts("[Error] not implemented");
}
 */
/*
static void compile_for_stmt_x86(struct ForStmt* m, struct Ctx* ctx){
    //TODO
    puts("[Error] not implemented");
}
 */
/*
static void compile_switch_stmt_x86(struct SwitchStmt* m, struct Ctx* ctx){
    //TODO
    puts("[Error] not implemented");
}
 */

static void compile_expr_x86(struct Expr* expr, struct Ctx* ctx){

    compile_unopterm_x86(expr->term1, ctx);
    //TODO
    puts("[Error] not implemented");
}

static void compile_unopterm_x86(struct UnOpTerm* uop, struct Ctx* ctx){

    compile_term_x86(uop->term, ctx);
    //TODO: apply the unary operator
    puts("[Error] not implemented");
}

static void compile_term_x86(struct Term* term, struct Ctx* ctx){

    switch (term->kind) {
        case 6: compile_variable_x86(term->ptr.m6, ctx); break;
        default:
            //TODO
            puts("[Error] not implemented");
            break;
    }
}

static void compile_variable_x86(struct Variable* var, struct Ctx* ctx){

    compile_simplevar_x86(var->simple_var, ctx);
    //TODO
    puts("[Error] not implemented");
}

static void compile_simplevar_x86(struct SimpleVar* s, struct Ctx* ctx){

    //TODO
    puts("[Error] not implemented");
    puts(s->name);
    printf("%d\n",ctx->error);
}