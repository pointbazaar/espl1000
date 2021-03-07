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

#include "token/TokenReader.h"
#include "token/token.h"

const char* FLAG_DEBUG = "-debug";
const char* FLAG_HELP = "-help";
const char* FLAG_TEST = "-test";

struct ParserFlags {

	bool debug;
	bool help;
	bool test;

	char* filename;
};

// --- private subroutines ---

bool main_inner(char* tokensFile, bool debug);

struct TokenList* readTokensFromTokensFile(FILE* file, char* tokensFile, bool debug);

void printHelp();

void build_ast_file(char* tokensFile, char* astJsonFile, bool debug);

struct ParserFlags* parseFlags(int argc, char** argv);

// --------------------------------

int main(int argc, char** argv){
	
	//this project is to parse a Dragon AST
	//from Tokens written into .tokens files by Dragon-Lexer,
	//and store it in a .ast file to be retrieved by the smalldragon transpiler
	//for simplicity, we invoke with just one filename as argument.
	//such we can easily have
	//multiple parallel invocations of the parser in the compiler.

	//setbuf(stdout,NULL);
	
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
	
	bool success = main_inner(flags->filename, flags->debug);
	
	free(flags);
	
	return (success)?0:1;
}

struct ParserFlags* parseFlags(int argc, char** argv){

	struct ParserFlags* flags = malloc(sizeof(struct ParserFlags));

	flags->help = false;
	flags->test = false;
	flags->debug = false;

	for(int i = 1; i < argc; i++){

		char* arg = argv[i];

		if(strcmp(FLAG_HELP,  arg) == 0){ flags->help  = true; }
		if(strcmp(FLAG_TEST,  arg) == 0){ flags->test  = true; }
		if(strcmp(FLAG_DEBUG, arg) == 0){ flags->debug = true; }

		if(arg[0] != '-'){
		
			flags->filename = arg;
		}
	}
	
	return flags;
}

void build_ast_file(char* tokensFile, char* astFile, bool debug) {

	if(debug){
		printf("Parser::build_ast_file(%s, %s, %d)\n", tokensFile, astFile, debug);
	}

	FILE* file = fopen(tokensFile, "r");
	
	if(file == NULL){
		printf("Parser.c:build_ast_file: could not open file: %s\n", tokensFile);
		exit(1);
	}
	
	struct TokenList* tokens = readTokensFromTokensFile(file, tokensFile, debug);
	
	fclose(file);

	if(debug){
		printf("Tokens as Source Code Fragment : \n");
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
		
		printf("there were tokens left when parsing.\n");
		list_print(tokens);
		printf("exiting.\n");

		freeTokenList(tokens);
		
		exit(1);
	}

	if(debug){ printf("write to %s\n", astFile); }

	write_ast(astFile,mynamespace);

	freeNamespace(mynamespace);
	freeTokenList(tokens);
}


bool main_inner(char* tokensFile, bool debug) {
	
	//returns false if it was unsuccessful

	if(debug){
		printf("Parser::main_inner\n");
		printf("Tokens File to parse: %s\n",tokensFile);
	}
	
	char fname1[32];
	strcpy(fname1, tokensFile);
	char* fname2 = fname1 + (strlen(fname1)-strlen(".tokens")) * sizeof(char);
	

	if(strcmp(fname2, ".tokens") != 0){
		printf("%s does not have .tokens extension. Exiting.\n", tokensFile);
		printf("actual extension: %s\n", fname2);
		return false;
	}

	FILE* f = fopen(tokensFile, "rw");
	
	if(f == NULL){
		printf("argument file %s does not exist.", tokensFile);
		return false;
	}

	fclose(f);
	char* AST_filename = malloc(sizeof(char)*100);
	strcpy(AST_filename, tokensFile);
	int l = strlen(tokensFile) - strlen(".tokens");
	AST_filename[l] = '\0';
	strcat(AST_filename, ".ast");

	build_ast_file(tokensFile,AST_filename,debug);
	free(AST_filename);
	
	return true;
}

void printHelp() {

	printf("dragon-parser FILE\n");
	printf("	dragon-parser - a parser for the dragon programming language\n\n");

	printf("EXAMPLES\n");
	printf("   dragon-parser .Main.dg.tokens\n");
	printf("   dragon-parser -debug .Main.dg.tokens\n\n");

	printf("GITHUB\n");
	printf("   https://github.com/pointbazaar/dragon-parser/\n\n");

	printf("AUTHOR\n");
	printf("	alex23667@gmail.com\n");
	printf("	David.Milosevic@web.de\n");
	printf("\n");

	printf("REPORTING BUGS\n");
	printf("   https://github.com/pointbazaar/dragon/issues\n\n");
		
}

struct TokenList* readTokensFromTokensFile(FILE* file, char* tokensFile, bool debug){

	if(debug){
		printf("readTokensFromTokensFile(%s, %d)\n", tokensFile, debug);
	}
	
	struct TokenList* tks = makeTokenList();
	strcpy(tks->relPath, tokensFile);
	
	size_t size = 50;
	char* line = malloc(sizeof(char)*size);
    
	while (getline(&line, &size, file)){
		
		line[strlen(line)-1] = '\0';
		
		bool isLineNo = false;
		struct Token* tkn = recognizeToken(line, &isLineNo, debug);
    	if(isLineNo){
			if(tkn != NULL){ 
				freeToken(tkn);
			}
			continue; 
		}
    	
    	if(tkn != NULL){
			list_add(tks, tkn);
    	}else{
    		break;
    	}
    }

	if(debug) {
		printf("read was successful\n");
		printf("done recognizing %d tokens\n", list_size(tks));
	}
	
	free(line);

	return tks;
}

