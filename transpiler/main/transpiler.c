#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "code_gen/c_code_gen/cg.h"
#include "flags/flags.h"
#include "util/fileutils/fileutils.h"
#include "invoke/invoke.h"
#include "transpiler.h"

bool transpileAndCompile(struct Flags* flags){
	
	for(int i=0;i < flags->count_filenames; i++){
	
		char* filename = flags->filenames[i];
	
		//invoke lexer, parser to generate .dg.ast file
		bool success = invoke_lexer_parser(filename, flags);

		if(!success){

			printf("[Error]: could not lex/parse %s.\n", filename);
			freeFlags(flags);

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
	char* c_filename = make_c_filename(flags->filenames[0]);
	char* h_filename = make_h_filename(flags->filenames[0]);

	bool success = transpileAndWrite(c_filename, h_filename, ast, flags);

	free_ast(ast);
	
	if(!success){ 
		
		free(c_filename);
		free(h_filename);
		return false; 
	}
	
	if(flags->has_main_fn){
		
		char* cmd_gcc = make_gcc_cmd(flags, c_filename);
		
		printf("%s\n", cmd_gcc);
		system(cmd_gcc);
		
		free(cmd_gcc);
	}
	
	free(c_filename);
	free(h_filename);
	
	return true;
}
