#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "Namespace.h"

#include "ast/io/ast_writer.h"

#include "ast/util/free_ast.h"

#include "token/reader/token_reader.h"
#include "token/token/token.h"
#include "token/list/TokenList.h"

#include "parser.h"

#define ERR_COULD_NOT_OPEN_FILE "[Parser] could not open file: %s\n"
#define ERR_FATAL "[Parser] Fatal.\n"
#define ERR_TOKENS_LEFT "[Parser] there were tokens left when parsing. Exiting.\n"

static char* extract_namespace_name(char* filename_tokens);
static char* extract_filename_ast(char* filename_tokens);

void build_ast_file(char *tokensFile) {

	FILE* file = fopen(tokensFile, "r");
	
	if(file == NULL){
		printf(ERR_COULD_NOT_OPEN_FILE, tokensFile);
		exit(1);
	}
	
	//full buffering for performance
	setvbuf(file, NULL, _IOFBF, BUFSIZ);
	
	struct TokenList* tokens = read_tokens_from_tokens_file(file, tokensFile);
	
	fclose(file);

	//get just the namespace name from .FILENAME.dg.tokens
	char* ns_name = extract_namespace_name(tokensFile);

	char* ast_filename = extract_filename_ast(tokensFile);

	struct Namespace* ns = makeNamespace(tokens, ast_filename, ns_name);
	
	free(ns_name);
	
	if(list_size(tokens) > 0){
        printf(ERR_TOKENS_LEFT);
        list_print(tokens);
        exit(1);
    }

	free(ast_filename);

	struct AST* ast = make(AST);
	
	ast->count_namespaces = 1;
	ast->namespaces       = malloc(sizeof(struct Namespace*)*1);
	ast->namespaces[0]    = ns;

	write_ast(ast);

	free_ast(ast);
	freeTokenList(tokens);
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
		printf(ERR_FATAL);
		exit(1);
	}
	
	index[0] = '\0';
	strcat(res, ".ast");
	
	return res;
}

