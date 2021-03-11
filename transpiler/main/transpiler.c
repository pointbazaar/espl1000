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

#include "code_gen/c_code_gen.h"
#include "flags/flags.h"
#include "util/help.h"

#include "util/fileutils/fileutils.h"

#include "invoke.h"

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
		printf("smalldragon v0.1.5\n");
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
	
	if(flags->debug){ printf("transpileAndCompile(...)\n"); }
	
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

	char* fname_out = make_c_filename(flags->filenames[0]);

	bool success = transpileAndWrite(fname_out, ast, flags);
	
	freeAST(ast);
	
	if(!success){ return false; }
	
	char* cmd_gcc = make_gcc_cmd(flags, fname_out);
	
	if(flags->has_main_fn){
		
		printf("%s\n", cmd_gcc);
		system(cmd_gcc);
		
		if(flags->avr){
			
			system("avr-gcc main.o -o main.elf");
			system("avr-objcopy -O ihex -j .text -j .data main.elf main.hex");
			
			//avr-objdump -D -m avr main.hex //to view contents
		}
	}
	
	free(cmd_gcc);
	free(fname_out);
	
	return true;
}
