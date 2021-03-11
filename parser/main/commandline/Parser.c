#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <malloc.h>

#include "Namespace.h"
#include "subr/Method.h"
#include "subr/DeclArg.h"
#include "statements/Stmt.h"
#include "types/Type.h"

#include "../../test/commandline/ParserTest.h"

#include "ast/io/ast_writer.h"

#include "ast/util/free_ast.h"

#include "token/reader/token_reader.h"
#include "token/token/token.h"

#include "parser_help.h"
#include "parser_flags.h"

// --- private subroutines ---

bool main_inner(struct ParserFlags* flags);

void printHelp();

void build_ast_file(char* tokensFile, char* astJsonFile, bool debug);

//this project is to parse a Dragon AST
//from Tokens written into .tokens files by Dragon-Lexer,
//and store it in a .ast file 
//to be retrieved by the smalldragon transpiler

int main(int argc, char** argv){
	
	mallopt(M_CHECK_ACTION, 3);

	struct ParserFlags* flags = parseFlags(argc, argv);

	if(flags->help) {
		
		printHelp();
		free(flags);
		return 0;
	}
	
	if(flags->test){
		
		free(flags);	
		return test_all(flags->debug);
	}
		
	if(flags->filename == NULL){
	
		printf("expected exactly 1 filename argument.\n");
		free(flags);
		exit(1);
	}
	
	bool success = main_inner(flags);
	
	free(flags);
	
	return (success)?0:1;
}

void build_ast_file(char* tokensFile, char* astFile, bool debug) {

	if(debug){
		printf("[Parser] build_ast_file(%s, %s, %d)\n", tokensFile, astFile, debug);
	}

	FILE* file = fopen(tokensFile, "r");
	
	if(file == NULL){
		printf("[Parser] build_ast_file: could not open file: %s\n", tokensFile);
		exit(1);
	}
	
	struct TokenList* tokens = readTokensFromTokensFile(file, tokensFile, debug);
	
	fclose(file);

	if(debug){
		printf("[Parser] Tokens as Source Code Fragment : \n");
		printf("%s\n", list_code(tokens, debug));
	}

	//get just the namespace name from .FILENAME.dg.tokens
	int l = strlen(".dg.tokens");
	int end = strlen(tokensFile) - l -1;
	char* namespaceName = tokensFile + 1;
	namespaceName[end] = '\0';

	struct Namespace* mynamespace = makeNamespace(tokens,namespaceName,debug);
	
	if(list_size(tokens) > 0){
	
		freeNamespace(mynamespace);
		
		printf("[Parser] there were tokens left when parsing.\n");
		list_print(tokens);
		printf("[Parser] exiting.\n");

		freeTokenList(tokens);
		
		exit(1);
	}

	if(debug){ printf("[Parser] write to %s\n", astFile); }

	write_ast(astFile,mynamespace);

	freeNamespace(mynamespace);
	freeTokenList(tokens);
}


bool main_inner(struct ParserFlags* flags) {
	
	char* tokensFile = flags->filename;

	if(flags->debug){
		printf("[Parser] main_inner\n");
		printf("[Parser] Tokens File to parse: %s\n",tokensFile);
	}
	
	char fname1[32];
	strcpy(fname1, tokensFile);
	char* fname2 = fname1 + (strlen(fname1)-strlen(".tokens")) * sizeof(char);
	

	if(strcmp(fname2, ".tokens") != 0){
		printf("[Parser] %s does not have .tokens extension. Exiting.\n", tokensFile);
		printf("[Parser] actual extension: %s\n", fname2);
		return false;
	}

	FILE* f = fopen(tokensFile, "rw");
	
	if(f == NULL){
		printf("[Parser] argument file %s does not exist.", tokensFile);
		return false;
	}

	fclose(f);
	char* AST_filename = malloc(sizeof(char)*100);
	
	strcpy(AST_filename, tokensFile);
	int l = strlen(tokensFile) - strlen(".tokens");
	AST_filename[l] = '\0';
	strcat(AST_filename, ".ast");

	build_ast_file(tokensFile,AST_filename, flags->debug);
	free(AST_filename);
	
	return true;
}
