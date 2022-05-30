#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "c_code_gen/cg.h"
#include "avr_code_gen/cg_avr.h"

#include "flags.h"
#include "util/fill_tables.h"
#include "util/fileutils/fileutils.h"

#include "util/ctx.h"
#include "invoke/invoke.h"
#include "compiler.h"
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

bool compile(struct Flags* flags){
	
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
	char* h_filename   = make_h_filename(flags->filenames[0]);
	char* asm_filename = make_asm_filename(flags->filenames[0]);

	bool success = false;

    struct Ctx* ctx = make(Ctx);

    ctx->tables = makeST(flags->debug);

    ctx->error = false;
    ctx->flags = flags;

    ctx->indent_level = 0;
    ctx->file         = NULL;
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


    success = compile_and_write_avr(asm_filename, ast, ctx);

    if(flags->emit_headers){
		success = transpile_and_write_c_headers( h_filename, ast, flags, ctx);
	}

	free_ast(ast);
	
	if(!success){
		free(h_filename);
		free(asm_filename);
		return false; 
	}

	int status = 0;

    char cmd[200];
    sprintf(cmd, "avra -o out %s", asm_filename);
    status = system(cmd);


	free(h_filename);
	free(asm_filename);
	
	return WEXITSTATUS(status) == 0;
}
