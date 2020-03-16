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
#include "ASTWriter.h"
#include "../../test/ParserTest.h"

int main(int argc, char** argv){
	//this project is to parse a Dragon AST
	//from Tokens written into .tokens files by Dragon-Lexer,
	//and store it in a .json file to be retrieved by the Dragon Compiler
	//for simplicity, we invoke with just one filename as argument.
	//such we can easily have
	//multiple parallel invocations of the parser in the compiler.

	setbuf(stdout,NULL);

	printf("dragon parser\n");

	vector<char*> flags;
	vector<char*> filenames;

	for(int i=1;i<argc;i++){
		char* arg = argv[i];
		if(argv[i][0] == '-'){
			flags.push_back(arg);
		}else{
			filenames.push_back(arg);
		}
	}

	bool debug = true;
	try {
		debug = find(flags.begin(),flags.end(),FLAG_DEBUG) != flags.end();
		if(debug){
			printf("Parser::main\n");
		}
		bool help = find(flags.begin(),flags.end(),FLAG_HELP) != flags.end();
		bool test = find(flags.begin(),flags.end(),FLAG_TEST) != flags.end();

		if(help) {
			printHelp();
		}else if(test){
			
			test_all(debug);
		}else{
			if(filenames.size() != 1){
				throw string("expected exactly 1 filename argument.");
			}
			main_inner(filenames[0],debug);
		}

	} catch (char* e) {
		cout << e << endl;
		exit(1);
	}
}

void build_ast_file(char* tokensFile, char* astJsonFile, bool debug) {

	if(debug){
		printf("Parser::build_ast_file\n");
	}

	struct TokenList* tokens = readTokensFromTokensFile(tokensFile,debug);

	printf("Tokens as Source Code Fragment : \n");
	printf("%s\n",tokens.code());

	//get just the namespace name from .FILENAME.dg.tokens
	char* namespaceName = tokensFile.substr(1,strlen(tokensFile) - strlen(".dg.tokens"));

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
		throw (tokensFile+" does not have .tokens extension. Exiting.");
	}
	*/

	ifstream f(tokensFile.c_str());

	if(f.good()) {
		f.close();
		string AST_filename = tokensFile.substr(0,tokensFile.size()-strlen(".tokens"))+".ast";

		build_ast_file(tokensFile,AST_filename,debug);
	}else {
		throw "argument file "+tokensFile+" does not exist.";
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

TokenList readTokensFromTokensFile(char* tokensFile, bool debug){

	char* fileNameWithoutPath = tokensFile;
	
	struct TokenList* tks = makeTokenList(tokensFile);
	ifstream file(tokensFile);
    char* str; 
    while (getline(file, str)){

		struct Token* tkn = recognizeToken(str, debug);
    	if(tkn != NULL){
			tks.add(tkn);
    	}
    }

	if(debug) {
		printf("read was successful\n");
		printf("done recognizing %d tokens\n",tks.size());
	}
	return tks;
}

