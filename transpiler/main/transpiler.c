#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <malloc.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "../test/test.h"

#include "code_gen/c_code_gen/cg.h"
#include "flags/flags.h"
#include "util/help.h"

#include "util/fileutils/fileutils.h"

#include "invoke/invoke.h"

#include "transpiler.h"

int main(int argc, char* argv[]){

	mallopt(M_CHECK_ACTION, 3);
	
	struct Flags* flags = makeFlags(argc, argv);
	
	if(flags->help){
		sd_print_help();
		freeFlags(flags);
		return 0;
	}
	
	if(flags->version){
		printf("smalldragon v0.2.0\n");
		freeFlags(flags);
		return 0;
	}
	
	if(flags->test){
		int status = transpiler_test_all(flags->debug);
		freeFlags(flags);
		return status;
	}
	
	bool success = transpileAndCompile(flags);
	
	freeFlags(flags);

	return (success)?0:1;
}

bool transpileAndCompile(struct Flags* flags){
	
	for(int i=0;i < flags->count_filenames; i++){
	
		char* filename = flags->filenames[i];
	
		//invoke lexer, parser to generate .dg.ast file
		bool success = invoke_lexer_parser(filename, flags);
		if(!success){
			printf("Error: could not lex/parse %s.\n", filename);
			freeFlags(flags);
			exit(1);
		}
	}
	
	struct AST* ast = invoke_ast_reader(flags);
	
	if(ast == NULL){ return false; }

	if(flags->count_filenames == 0){
		printf("[Error] expected atleast 1 filename\n");
		exit(1);
	}

	//output filenames
	char* c_filename = make_c_filename(flags->filenames[0]);
	char* h_filename = make_h_filename(flags->filenames[0]);

	bool success = transpileAndWrite(c_filename, h_filename, ast, flags);
	
	freeAST(ast);
	
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
