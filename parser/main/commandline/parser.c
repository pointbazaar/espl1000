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
#include "token/list/TokenList.h"

#include "parser_help.h"
#include "parser_flags.h"

#define ERR_FILE_NOT_EXIST      "[Parser] file %s does not exist.\n"
#define ERR_COULD_NOT_OPEN_FILE "[Parser] could not open file: %s\n"
#define ERR_EXPECT_1_FILENAME   "[Parser] expected exactly 1 filename argument.\n"

//this project is to parse a Dragon AST
//from Tokens written into .tokens files by Dragon-Lexer,
//and store it in a .ast file 
//to be retrieved by the smalldragon transpiler

// --- private subroutines ---

bool main_inner(struct ParserFlags* flags);

void printHelp();

void build_ast_file(char* tokensFile, bool debug);

static void exit_incomplete_parse(struct TokenList* tokens);

static char* extract_namespace_name(char* filename_tokens);
static char* extract_filename_ast(char* filename_tokens);

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
	
		printf(ERR_EXPECT_1_FILENAME);
		free(flags);
		exit(1);
	}
	
	bool success = main_inner(flags);
	
	free(flags);
	
	return (success)?0:1;
}

void build_ast_file(char* tokensFile, bool debug) {

	FILE* file = fopen(tokensFile, "r");
	
	if(file == NULL){
		printf(ERR_COULD_NOT_OPEN_FILE, tokensFile);
		exit(1);
	}
	
	//full buffering for performance
	setvbuf(file, NULL, _IOFBF, BUFSIZ);
	
	struct TokenList* tokens = readTokensFromTokensFile(file, tokensFile, debug);
	
	fclose(file);

	//get just the namespace name from .FILENAME.dg.tokens
	char* ns_name = extract_namespace_name(tokensFile);

	char* ast_filename = extract_filename_ast(tokensFile);

	struct Namespace* ns = makeNamespace(tokens, ast_filename, ns_name,debug);
	
	free(ns_name);
	
	if(list_size(tokens) > 0){ exit_incomplete_parse(tokens); }

	if(debug){ printf("[Parser] write to %s\n", ast_filename); }
	free(ast_filename);

	struct AST* ast = make(AST);
	
	ast->count_namespaces = 1;
	ast->namespaces       = malloc(sizeof(struct Namespace*)*1);
	ast->namespaces[0]    = ns;

	writeAST(ast);

	freeAST(ast);
	freeTokenList(tokens);
}

static void exit_incomplete_parse(struct TokenList* tokens){
	
	printf("[Parser] there were tokens left when parsing.\n");
	list_print(tokens);
	printf("[Parser] exiting.\n");
	exit(1);
}

static char* extract_namespace_name(char* filename_tokens){
	
	//?.FILENAME.dg.tokens
	// ^        ^
	//dot1      dot2
	
	char* last = filename_tokens + strlen(filename_tokens) - 1;
	
	while(last[0] != '.'){ last--; }
	last--;
	
	while(last[0] != '.'){ last--; }
	char* dot2 = last;
	last--;
	
	while(last[0] != '.'){ last--; }
	char* dot1 = last;
	last--;
	
	int l = (int)(dot2-dot1);
	
	if(l > DEFAULT_STR_SIZE){ l = DEFAULT_STR_SIZE; }
	
	char* res = malloc(DEFAULT_STR_SIZE);
	memset(res, 0, DEFAULT_STR_SIZE);
	strncpy(res, dot1+1, l-1);
	
	return res;
}

static char* extract_filename_ast(char* filename_tokens){
	
	char* res = malloc(strlen(filename_tokens)+1);
	
	strcpy(res, filename_tokens);
	
	char* index = strrchr(res, '.');
	
	if(index == NULL){ 
		printf("Fatal\n");
		exit(1);
	}
	
	index[0] = '\0';
	strcat(res, ".ast");
	
	return res;
}

bool main_inner(struct ParserFlags* flags) {
	
	char* tokensFile = flags->filename;
	
	char* extension = strrchr(tokensFile, '.');
	
	if(extension == NULL){ 
		printf("Fatal\n");
		exit(1);
	}
	
	if(strcmp(extension, ".tokens") != 0){
		printf("[Parser] %s does not have .tokens extension. Exiting.\n", tokensFile);
		printf("[Parser] actual extension: %s\n", extension);
		return false;
	}

	FILE* f = fopen(tokensFile, "r");
	if(f == NULL){
		printf(ERR_FILE_NOT_EXIST, tokensFile);
		return false;
	}
	fclose(f);

	build_ast_file(tokensFile, flags->debug);
	
	return true;
}
