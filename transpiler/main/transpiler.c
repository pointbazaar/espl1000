#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "transpiler.h"
#include "../../ast/ast_reader.h"
#include "../../ast/ast.h"
#include "c_code_gen.h"

int main(int argc, char* argv[]){

	char* filename = NULL;
	bool debug = false;
	bool test  = false;
	
	//if there are more than 100, it can exit
	const int gcc_flags_max = 100;
	char** gcc_flags = malloc(sizeof(char*)*gcc_flags_max);
	int gcc_flags_count = 0;
	
	for(int i=1; i < argc; i++){
		char* arg = argv[i];
		if(arg[0] == '-'){
			//arg is a flag
			if(strcmp(arg, "-debug") == 0){
				debug = true;
			}else if(strcmp(arg, "-test") == 0){
				test = true;
			}else{
				//pass this flag when calling gcc
				gcc_flags[gcc_flags_count] = arg;
				gcc_flags_count++;
				
				if(gcc_flags_count > gcc_flags_max){
					printf("Exceeded max amount");
					printf(" of gcc flags to pass through.\n");
					exit(1);
				}
				
			}
		}else{
			filename = arg;
		}
	}
	
	if(debug){
		printf("smalldragon v0.01\n");
	}
	
	if(test){
		printf("smalldragon/transpiler currently has no tests.\n");
		exit(0);
	}
	
	if(filename == NULL){
		printf("expected at least 1 filename\n");
		exit(1);
	}
	
	check_dg_extension(filename);
	
	//invoke lexer, parser to generate .dg.ast file
	invoke_lexer_parser(filename, debug);

	char ast_filename[100];
	char fnamecpy[100];
	strcpy(fnamecpy, filename);
	
	char* base_name = basename(fnamecpy);
	char* dir_name = dirname(fnamecpy);
	sprintf(ast_filename, "%s/.%s.ast", dir_name, base_name);
	
	if(debug){
		printf("try to open file %s\n", ast_filename);
	}

	//parse AST
	struct AST_Whole_Program* ast = readAST(ast_filename, debug);

	char fname_out[32]; //new output filename

	strcpy(fname_out, filename);
	//remove the '.dg'
	fname_out[strlen(fname_out)-3] = '\0';
	strcat(fname_out, ".c");

	//transpile to C code 
	//and write to file 
	transpileAndWrite(fname_out, ast, debug);
	
	char cmd_gcc[500];
	strcpy(cmd_gcc, "gcc -o main ");
	strcat(cmd_gcc, fname_out);
	
	//we assume here cmd_gcc will never exceed it's allocated size.
	//Warning. this is a stupid assumption.
	for(int i=0; i < gcc_flags_count; i++){
		strcat(cmd_gcc, " ");
		strcat(cmd_gcc, gcc_flags[i]);
	}
	
	//compile with gcc
	system(cmd_gcc);

	exit(0);
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
	
	if(debug){
		strcpy(cmd1, "dragon-lexer-debug ");
	}else{
		strcpy(cmd1, "dragon-lexer ");
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
