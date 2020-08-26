#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "Parser.h"
#include "TokenReader.h"
#include "../parsing/Namespace.h"
#include "../parsing/Method.h"
#include "../parsing/DeclArg.h"
#include "../parsing/statements/Stmt.h"
#include "../parsing/typenodes/Type.h"
#include "../../../../ast/ASTWriter.h"
#include "../../test/ParserTest.h"

#include "../../../../util/util.h"

int main(int argc, char** argv){
	//this project is to parse a Dragon AST
	//from Tokens written into .tokens files by Dragon-Lexer,
	//and store it in a .json file to be retrieved by the Dragon Compiler
	//for simplicity, we invoke with just one filename as argument.
	//such we can easily have
	//multiple parallel invocations of the parser in the compiler.

	//setbuf(stdout,NULL);

	char** filenames = smalloc(sizeof(char*)*100);
	int filenamescount = 0;
	
	char** flags = smalloc(sizeof(char*)*100);
	int flagscount = 0;
	
	//read flags, filenames

	for(int i=1;i<argc;i++){
		char* arg = argv[i];
		if(argv[i][0] == '-'){
			flags[flagscount++] = arg;
		}else{
			filenames[filenamescount++] = arg;
		}
	}

	bool debug = false;
	bool help = false;
	bool test = false;

	
	//apply flags
	
	
	for(int i=0;i<flagscount;i++){
		if(strcmp(FLAG_HELP, flags[i])==0){
			help = true;
		}
		if(strcmp(FLAG_TEST, flags[i])==0){
			test = true;
		}
		if(strcmp(FLAG_DEBUG, flags[i])==0){
			debug = true;
			printf("DEBUG ENABLED\n");
		}
	}
	
	if(debug){
		printf("--- dragon-parser ---\n");
	}

	if(help) {
		printHelp();
	}else if(test){
		
		free(flags);
		free(filenames);
		
		return test_all(debug);
	}else{
		if(filenamescount != 1){
			printf("expected exactly 1 filename argument.\n");
			exit(1);
		}
		main_inner(filenames[0],debug);
	}
	
	free(flags);
	free(filenames);

	exit(0);
}

void build_ast_file(char* tokensFile, char* astJsonFile, bool debug) {

	if(debug){
		printf("Parser::build_ast_file(%s, %s, %d)\n", tokensFile, astJsonFile, debug);
	}

	struct TokenList* tokens = readTokensFromTokensFile(tokensFile,debug);

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

	if(debug){
		printf("write to %s\n",astJsonFile);
	}

	write_ast(astJsonFile,mynamespace);
}


void main_inner(char* tokensFile, bool debug) {

	if(debug){
		printf("Parser::main_inner\n");
		printf("Tokens File to parse: %s\n",tokensFile);
	}

	//TODO: re-enable this check later
	/*
	if(!tokensFile.endsWith(".tokens")){
		printf(tokensFile+" does not have .tokens extension. Exiting.");
		exit(1);
	}
	*/

	FILE* f = fopen(tokensFile, "rw");

	if(f != NULL) {
		fclose(f);
		char* AST_filename = smalloc(sizeof(char)*100);
		strcpy(AST_filename, tokensFile);
		int l = strlen(tokensFile) - strlen(".tokens");
		AST_filename[l] = '\0';
		strcat(AST_filename, ".ast");

		build_ast_file(tokensFile,AST_filename,debug);
	}else {
		printf("argument file %s does not exist.", tokensFile);
		exit(1);
	}

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
	printf("	alex23667@gmail.com\n\n");

	printf("REPORTING BUGS\n");
	printf("   https://github.com/pointbazaar/dragon/issues\n\n");
		
}

struct TokenList* readTokensFromTokensFile(char* tokensFile, bool debug){

	if(debug){
		printf("readTokensFromTokensFile(%s, %d)\n", tokensFile, debug);
	}
	
	struct TokenList* tks = makeTokenList();
	strcpy(tks->relPath, tokensFile);
	FILE* file = fopen(tokensFile,"r");
	size_t size = 50;
    char* line = smalloc(sizeof(char)*size);
    
    while (getline(&line, &size, file)){
		
		line[strlen(line)-1] = '\0';
		
		bool isLineNo = false;
		struct Token* tkn = recognizeToken(line, &isLineNo, debug);
    	if(isLineNo){ continue; }
    	
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
	return tks;
}

