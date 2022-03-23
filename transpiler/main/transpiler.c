#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "code_gen/c_code_gen/cg.h"
#include "code_gen/x86/cg_x86.h"
#include "code_gen/avr/cg_avr.h"

#include "flags.h"
#include "util/fill_tables.h"
#include "util/fileutils/fileutils.h"

#include "util/ctx.h"
#include "invoke/invoke.h"
#include "transpiler.h"
#include "tables/symtable/symtable.h"
#include "lambda/transpile_lambdas.h"

#include "typechecker/typecheck.h"
#include "tables/sst/sst_fill.h"
#include "analyzer/fn/fn_analyzer.h"
#include "analyzer/dead/dead_analyzer.h"
#include "analyzer/halts/halt_analyzer.h"
#include "analyzer/annotation/annotation_analyzer.h"

static void backfill_lambdas_into_sst(struct AST* ast, struct ST* st);

static void backfill_lambdas_into_sst(struct AST* ast, struct ST* st){

    for(int i=0; i < ast->count_namespaces; i++){

        struct Namespace* ns = ast->namespaces[i];

        for(int j = 0; j < ns->count_methods; j++){

            struct Method* m = ns->methods[j];

            //name starts with lambda_
            if(strncmp(m->decl->name, "lambda_", strlen("lambda_")) != 0){
                continue;
            }

            struct Type* t = method_to_type(m);
            st_register_inferred_type(st, t);

            struct SSTLine* line = makeSSTLine2(m, t, ns->name);

            sst_add(st->sst, line);
        }
    }
}

bool transpileAndCompile(struct Flags* flags){
	
	for(int i=0;i < flags->count_filenames; i++){
	
		char* filename = flags->filenames[i];
	
		//invoke lexer, parser to generate .dg.ast file
		bool success = invoke_lexer_parser(filename);

		if(!success){

			printf("[Error]: could not lex/parse %s.\n", filename);
			return false;
		}
	}
	
	struct AST* ast = invoke_ast_reader(flags);
	
	if(ast == NULL){ return false; }

	if(flags->count_filenames == 0){

		printf("[Error] expected atleast 1 filename\n");
		free_ast(ast);
        return false;
	}

	//output filenames
	char* c_filename   = make_c_filename(flags->filenames[0]);
	char* h_filename   = make_h_filename(flags->filenames[0]);
	char* asm_filename = make_asm_filename(flags->filenames[0]);

	bool success = false;

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

    fill_tables(ast, ctx);
    transpileLambdas(ast, ctx->tables);
    //RE-FILL the newly created lambda functions
    backfill_lambdas_into_sst(ast, ctx->tables);
    struct TCError* errors = typecheck_ast(ast, ctx->tables, true);
    bool checks = errors == NULL;
    if(!checks){
        ctx->error = true;
        return false;
    }

    analyze_functions(ctx->tables, ast);
    analyze_dead_code(ctx->tables, ast);
    analyze_termination(ctx->tables);
    analyze_annotations(ctx->tables, ast);

    if(flags->avr){
        success = compile_and_write_avr(asm_filename, ast, flags, ctx);
    }else if(flags->x86){
		success = compile_and_write_x86(asm_filename, ast, flags);
	}else{
		success = transpile_and_write_c_code(c_filename, h_filename, ast, flags, ctx);
	}

	free_ast(ast);
	
	if(!success){
		free(c_filename);
		free(h_filename);
		free(asm_filename);
		return false; 
	}

	int status = 0;

	if(flags->x86){
		char cmd[200];
		sprintf(cmd, "nasm %s", asm_filename);
		status = system(cmd);

	}else if(flags->avr){
        char cmd[200];
        sprintf(cmd, "avra -o out %s", asm_filename);
        status = system(cmd);

    }else if(flags->has_main_fn){
        char* cmd_gcc = make_gcc_cmd(flags, c_filename);

        status = system(cmd_gcc);

        free(cmd_gcc);
    }
	
	free(c_filename);
	free(h_filename);
	free(asm_filename);
	
	return WEXITSTATUS(status) == 0;
}
