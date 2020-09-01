#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "../../ast/ast_reader.h"
#include "../../ast/ast.h"
#include "../test/test.h"

#include "c_code_gen.h"
#include "flags.h"
#include "help.h"

#include "transpiler.h"

int main(int argc, char* argv[]){

	char* filename = NULL;
	
	struct Flags* flags = makeFlags(argc, argv);
	
	for(int i=1; i < argc; i++){
		
		if(argv[i][0] != '-'){
			
			filename = argv[i];
		}
	}
	
	
	
	if(flags->help){
		sd_print_help();
		freeFlags(flags);
		return 0;
	}
	
	if(flags->version){
		printf("smalldragon v0.0.3\n");
		printf("built on %s\n", ctime);
		
		freeFlags(flags);
		return 0;
	}
	
	if(flags->test){
		int status = transpiler_test_all(flags->debug);
		freeFlags(flags);
		return status;
	}
	
	if(filename == NULL){
		printf("expected at least 1 filename\n");
		freeFlags(flags);
		exit(1);
	}
	
	transpileAndCompile(filename, flags);
	
	freeFlags(flags);

	return 0;
}

void check_dg_extension(char* filename){
	const int ext_index = strlen(filename)-3;
	if(strcmp(filename+ext_index, ".dg") != 0){
			printf("filename has to have .dg extension\n");
			exit(1);
	}
}

void invoke_lexer_parser(char* filename, bool debug){
	
	char cmd1[100];
	
	strcpy(cmd1, "dragon-lexer ");
	
	if(debug){
		strcat(cmd1, "-debug ");
	}
	strcat(cmd1, filename);
	
	if(debug){
		printf("DEBUG: executing: %s\n", cmd1);
	}
	system(cmd1);
	
	char fnamecpy[100];
	strcpy(fnamecpy, filename);
	
	char* base_name = basename(fnamecpy);
	char* dir_name = dirname(fnamecpy);
	
	char cmd2[100];
	sprintf(
		cmd2, 
		"dragon-parser %s %s/.%s.tokens", 
		(debug)?"-debug":"",
		dir_name,
		base_name
	);
	
	if(debug){
		printf("DEBUG: executing: %s\n", cmd2);
	}
	system(cmd2);
}

void transpileAndCompile(
	char* filename, 
	struct Flags* flags
){
	if(flags->debug){ printf("transpileAndCompile(...)\n"); }
	
	check_dg_extension(filename);
	
	//invoke lexer, parser to generate .dg.ast file
	invoke_lexer_parser(filename, flags->debug);

	char ast_filename[100];
	char fnamecpy[100];
	strcpy(fnamecpy, filename);
	
	char* base_name = basename(fnamecpy);
	char* dir_name = dirname(fnamecpy);
	sprintf(ast_filename, "%s/.%s.ast", dir_name, base_name);
	
	if(flags->debug){
		printf("try to open file %s\n", ast_filename);
	}

	//parse AST
	struct AST_Whole_Program* ast = readAST(ast_filename, flags->debug);

	char fname_out[DEFAULT_STR_SIZE]; //new output filename

	strcpy(fname_out, filename);
	//remove the '.dg'
	fname_out[strlen(fname_out)-3] = '\0';
	strcat(fname_out, ".c");

	//transpile to C code 
	//and write to file 
	transpileAndWrite(fname_out, ast, flags);
	
	char cmd_gcc[500];
	strcpy(cmd_gcc, "");
	
	if(flags->avr){
		//choose -mmcu=attiny25, but you can replace with
		//whatever you like
		//we chase attiny25 to have it generate less complex instructions
		strcat(cmd_gcc, "avr-gcc -o main.o -I /usr/share/avra -mmcu=attiny45 ");
	}else{
		//-Wall enabled so we catch
		//our transpiler if it generates code
		//that could cause trouble.
		//Also serves as feedback for users,
		//as they smalldragon code could transpile
		//to C code that causes warnings.
		strcat(cmd_gcc, "gcc -Wall -o a.out ");
	}
	
	strcat(cmd_gcc, fname_out);
	
	//we assume here cmd_gcc will never exceed it's allocated size.
	//Warning. this is a stupid assumption.
	for(int i=0; i < flags->gcc_flags_count; i++){
		strcat(cmd_gcc, " ");
		strcat(cmd_gcc, flags->gcc_flags[i]);
	}
	
	//compile with gcc
	if(flags->debug){
		printf("%s\n", cmd_gcc);
	}
	system(cmd_gcc);
	
	if(flags->avr){
		//use various tools to compile a .hex file
		//which can be read by e.g. mdx
		
		system("avr-gcc main.o -o main.elf");
		system("avr-objcopy -O ihex -j .text -j .data main.elf main.hex");
		
		//use 
		//avr-objdump -D -m avr main.hex
		//to view contents
	}
}
