#define _XOPEN_SOURCE

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#include "ast/io/ast_reader.h"
#include "util/fileutils/fileutils.h"
#include "invoke.h"

bool invoke_lexer_parser(char* filename, struct Flags* flags){
	
	char cmd1[100];
	
	strcpy(cmd1, "dragon-lexer ");
	
	if(flags->clean){ strcat(cmd1, "-clean "); }

	strcat(cmd1, filename);

	printf("%s\n", cmd1);

	int status = system(cmd1);
	if(WEXITSTATUS(status) != 0){
		printf("Error: lexer exited with nonzero exit code\n");
		return false;
	}
	
	char fnamecpy[100];
	strcpy(fnamecpy, filename);
	
	char* base_name = basename(fnamecpy);
	char* dir_name = dirname(fnamecpy);
	
	char cmd2[100];
	sprintf(
		cmd2, 
		"dragon-parser %s/.%s.tokens", 
		dir_name,
		base_name
	);	
	printf("%s\n", cmd2);

	status = system(cmd2);
	if(WEXITSTATUS(status) != 0){
		printf("Error: parser exited with nonzero exit code\n");
		return false;
	}

	return true;
}

struct AST* invoke_ast_reader(struct Flags* flags){
	
	char* ast_filenames[flags->count_filenames];
	
	for(int i = 0; i < flags->count_filenames; i++){
		
		ast_filenames[i] = 
			make_ast_filename(flags->filenames[i]);
	}

	struct AST* ast = readAST(ast_filenames, flags->count_filenames, flags->debug);
	
	for(int i = 0; i < flags->count_filenames; i++){
	
		free(ast_filenames[i]);
	}
	
	return ast;
}
