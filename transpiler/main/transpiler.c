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
	
	for(int i=1; i < argc; i++){
		char* arg = argv[i];
		if(strcmp(arg, "-debug") == 0){
			debug = true;
		}else{
			filename = arg;
		}
	}
	
	if(debug){
		printf("smalldragon v0.01\n");
	}
	
	if(filename == NULL){
		printf("expected at least 1 filename\n");
		exit(1);
	}
	
	check_dg_extension(filename);
	
	//invoke lexer, parser to generate .dg.ast file
	invoke_lexer_parser(filename, debug);

	char ast_filename[100];
	char* base_name = basename(filename);
	char* dir_name = dirname(filename);
	sprintf(ast_filename, "%s/.%s.ast", dir_name, base_name);
	
	printf("try to open file %s\n", ast_filename);

	//parse AST
	struct AST_Whole_Program* ast = readAST(ast_filename, debug);

	if(ast == NULL){
		printf("Error parsing from %s\n", ast_filename);
		exit(1);
	}

	char fname_out[32]; //new output filename

	strcpy(fname_out, filename);
	//remove the '.dg'
	fname_out[strlen(fname_out)-3] = '\0';
	strcat(fname_out, ".c");

	//TODO

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
		printf("executing: %s\n", cmd1);
	}
	system(cmd1);
	
	char* base_name = basename(filename);
	char* dir_name = dirname(filename);
	
	char cmd2[100];
	sprintf(
		cmd2, 
		"dragon-parser %s %s/.%s.tokens", 
		(debug)?"-debug":"",
		dir_name,
		base_name
	);
	
	system(cmd2);
}
