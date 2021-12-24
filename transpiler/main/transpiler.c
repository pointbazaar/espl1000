#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "code_gen/c_code_gen/cg.h"
#include "code_gen/x86/cg_x86.h"

#include "flags.h"
#include "util/fileutils/fileutils.h"
#include "invoke/invoke.h"
#include "transpiler.h"
#include "code_gen/avr/cg_avr.h"

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
    if(flags->avr){
        success = compile_and_write_avr(asm_filename, ast, flags);
    }else if(flags->x86){
		success = compile_and_write_x86(asm_filename, ast, flags);
	}else{
		success = transpile_and_write_c_code(c_filename, h_filename, ast, flags);
	}

	free_ast(ast);
	
	if(!success){
		free(c_filename);
		free(h_filename);
		free(asm_filename);
		return false; 
	}

	int status = 0;
	
	if(flags->has_main_fn && !flags->x86){
		
		char* cmd_gcc = make_gcc_cmd(flags, c_filename);

		status = system(cmd_gcc);
		
		free(cmd_gcc);
	}

	if(flags->x86){
		char cmd[200];
		sprintf(cmd, "nasm %s", asm_filename);
		status = system(cmd);

	}
	
	free(c_filename);
	free(h_filename);
	free(asm_filename);
	
	return WEXITSTATUS(status) == 0;
}
