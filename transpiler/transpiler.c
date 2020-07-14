#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "transpiler.h"
#include "../ast/ast_reader.h"
#include "../ast/ast.h"
#include "java_code_gen.h"

int main(int argc, char* argv[]){
	
	printf("smalldragon v0.01\n");

	//TODO: consider flags

	char* filename;
	bool debug = false;
	
	for(int i=1; i < argc; i++){
		char* arg = argv[i];
		if(strcmp(arg, "-debug") == 0){
			debug = true;
		}else{
			filename = arg;
		}
	}
	
	check_dg_extension(filename);
	
	//invoke lexer, parser to generate .dg.ast file
	invoke_lexer_parser(filename, debug);

	char ast_filename[100];
	sprintf(ast_filename, ".%s.ast", filename);
	
	printf("try to open file %s\n", ast_filename);

	//parse AST
	struct AST_Whole_Program* ast = readAST(ast_filename, debug);

	if(ast == NULL){
		printf("Error parsing from %s\n", ast_filename);
		exit(1);
	}

	char fname_out[32]; //new output filename

	strcpy(fname_out, filename);
	strcat(fname_out, ".java");

	//TODO: create symbol tables
	
	gen_java_code(ast, fname_out);

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
	
	system(cmd1);
	
	char cmd2[100];
	sprintf(
		cmd2, 
		"dragon-parser %s .%s.tokens", 
		(debug)?"-debug":"",
		filename
	);
	
	system(cmd2);
}
