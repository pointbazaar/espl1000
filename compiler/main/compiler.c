#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "avr_code_gen/cg_avr.h"

#include "../cli/flags/flags.h"
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

    if(flags_count_filenames(flags) == 0){
        printf("[Error] expected atleast 1 filename\n");
        return false;
    }

    struct AST* ast = make(AST);
    ast->count_namespaces = flags_count_filenames(flags);
    ast->namespaces = exit_malloc(sizeof(struct Namespace*)*flags_count_filenames(flags));
	
	for(int i=0;i < flags_count_filenames(flags); i++){
	
		char* filename = flags_filenames(flags,i);

        int status = invoke_lexer(filename);

        if (WEXITSTATUS(status) != 0) {
            printf("[Error] lexer exited with nonzero exit code\n");
            return false;
        }

        struct Namespace* ns = invoke_parser(filename);

        if (ns == NULL) {
            printf("[Error] parser exited with nonzero exit code\n");
            return false;
        }

        ast->namespaces[i] = ns;
	}

    struct Ctx* ctx = ctx_ctor(flags, makeST(flags_debug(flags)));

    fill_tables(ast, ctx);
    transpileLambdas(ast, ctx_tables(ctx));
    //RE-FILL the newly created lambda functions
    backfill_lambdas_into_sst(ast, ctx_tables(ctx));
    
    
    struct TCError* errors = typecheck_ast(ast, ctx_tables(ctx), true);
	
    if(errors != NULL){
        return false;
    }

    analyze_functions(ctx_tables(ctx), ast);
    analyze_dead_code(ctx_tables(ctx), ast);
    analyze_termination(ctx_tables(ctx));
    analyze_annotations(ctx_tables(ctx), ast);


    bool success = compile_and_write_avr(ast, ctx);

	free_ast(ast);
	
	if(!success){
		return false; 
	}

	int status = 0;

    char cmd[200];
    sprintf(cmd, "avra %s > /tmp/avra-stdout 2> /tmp/avra-stderr", flags_asm_filename(flags));
    status = system(cmd);

	if (WEXITSTATUS(status) != 0) {
		printf("[Error] error with avra, see /tmp/avra-stdout, /tmp/avra-stderr \n");
	}
	
	ctx_dtor(ctx);
	
	return WEXITSTATUS(status) == 0;
}
