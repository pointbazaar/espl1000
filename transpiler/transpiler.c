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
	if(argc != 2){
		printf("Exactly 1 filename required as argument.\n");
		exit(1);
	}

	char* filename = argv[1];
	
	int ext_index = strlen(filename)-3;
	if(strcmp(filename+ext_index, ".dg") != 0){
			printf("filename has to have .dg extension\n");
			exit(1);
	}

	printf("try to open file %s\n", filename);

	//parse AST
	struct AST_Whole_Program* ast = readAST(filename);

	if(ast == NULL){
		printf("Error parsing from %s\n", filename);
		exit(1);
	}

	char fname_out[32]; //new output filename

	strcpy(fname_out, filename);
	//we expect '.' somewhere inside
	for(int i=0;i<strlen(fname_out);i++){
		if(fname_out[i] == '.'){
			fname_out[i] = '\0';
		}
	}
	strcat(fname_out, ".java");

	//TODO: create symbol tables

	
	gen_java_code(ast, fname_out);

	exit(0);
}
