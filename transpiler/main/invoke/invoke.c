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

static int invoke_lexer (char* filename, struct Flags* flags);
static int invoke_parser(char* filename, struct Flags* flags);

bool invoke_lexer_parser(char* filename, struct Flags* flags){
	
	int status = invoke_lexer(filename, flags);
	
	if(WEXITSTATUS(status) != 0){
		printf("[Transpiler][Error] lexer exited with nonzero exit code\n");
		return false;
	}
	
	status = invoke_parser(filename, flags);

	if(WEXITSTATUS(status) != 0){
		printf("[Transpiler][Error] parser exited with nonzero exit code\n");
		return false;
	}

	return true;
}

static int invoke_lexer(char* filename, struct Flags* flags){
	
	char* cmd1 = malloc(strlen(filename)+100);
	
	strcpy(cmd1, "dragon-lexer ");

	strcat(cmd1, filename);

	int status = system(cmd1);
	free(cmd1);
	return status;
}

static int invoke_parser(char* filename, struct Flags* flags){
	
	char* fnamecpy = malloc(strlen(filename)+1);
	strcpy(fnamecpy, filename);
	
	char* base_name = basename(fnamecpy);
	char* dir_name  = dirname(fnamecpy);
	
	char* cmd2 = malloc(strlen(filename)+100);
	
	sprintf(cmd2, "dragon-parser %s/.%s.tokens", dir_name, base_name);	
	
	int status = system(cmd2);
	
	free(cmd2);
	free(fnamecpy);
	
	return status;
}

struct AST* invoke_ast_reader(struct Flags* flags){
	
	char* ast_filenames[flags->count_filenames];
	
	for(int i = 0; i < flags->count_filenames; i++){
		
		ast_filenames[i] = 
			make_ast_filename(flags->filenames[i]);
	}

	struct AST* ast = read_ast(ast_filenames, flags->count_filenames);
	
	for(int i = 0; i < flags->count_filenames; i++){
	
		free(ast_filenames[i]);
	}
	
	return ast;
}
