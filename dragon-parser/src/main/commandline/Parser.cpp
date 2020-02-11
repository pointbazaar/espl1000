//standard includes
#include <vector> 
#include <stdio.h>
#include <map>
#include <stdbool.h>
#include <string>
#include <optional>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

#include <stdlib.h>
#include <time.h>


//project includes
#include "Parser.hpp"
#include "TokenReader.hpp"
#include "../parsing/Namespace.hpp"
#include "../parsing/Method.hpp"
#include "../parsing/DeclArg.hpp"
#include "../parsing/statements/Stmt.hpp"
#include "../parsing/typenodes/Type.hpp"
#include "ASTWriter.hpp"

using namespace std;

int main(int argc, char** argv){
	//this project is to parse a Dragon AST
	//from Tokens written into .tokens files by Dragon-Lexer,
	//and store it in a .json file to be retrieved by the Dragon Compiler
	//for simplicity, we invoke with just one filename as argument.
	//such we can easily have
	//multiple parallel invocations of the parser in the compiler.

	cout << "dragon parser" << endl;

	vector<string> flags;
	vector<string> filenames;

	for(int i=1;i<argc;i++){
		string arg = string(argv[i]);
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
			cout << "Parser::main" << endl;
		}
		bool help = find(flags.begin(),flags.end(),FLAG_HELP) != flags.end();
		bool test = find(flags.begin(),flags.end(),FLAG_TEST) != flags.end();

		if(help) {
			printHelp();
		}else if(test){
			//TODO: re-enable later
			//ParserTest.test_all();
		}else{
			if(filenames.size() != 1){
				throw "expected exactly 1 filename argument.";
			}
			main_inner(filenames.at(0),debug);
		}

	} catch (string e) {
		cout << e << endl;
		exit(1);
	}
}

void build_ast_file(string tokensFile, string astJsonFile, bool debug) {

	if(debug){
		cout << "Parser::build_ast_file" << endl;
	}

	TokenList tokens = readTokensFromTokensFile(tokensFile,debug);

	cout << "Tokens as Source Code Fragment : " << endl;
	cout << tokens.code() << endl << endl;

	//get just the namespace name from .FILENAME.dg.tokens
	string namespaceName = tokensFile.substr(1,string(tokensFile).size() - string(".dg.tokens").size());

	Namespace mynamespace = Namespace(&tokens,namespaceName,debug);

	if(debug){
		cout << "write to "+astJsonFile << endl;
	}

	write_ast(astJsonFile,mynamespace);
}


void main_inner(string tokensFile, bool debug) {

	if(debug){
		cout << "Parser::main_inner" << endl;
		cout << "Tokens File to parse: "+tokensFile << endl;
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
		string AST_filename = tokensFile.substr(0,tokensFile.size()-string(".tokens").size())+".ast";

		build_ast_file(tokensFile,AST_filename,debug);
	}else {
		throw ("argument file "+tokensFile+" does not exist.");
	}

}

void printHelp() {

	cout << "dragon-parser FILE" << endl
		<< "	dragon-parser - a parser for the dragon programming language" << endl

	<< "EXAMPLES" << endl
	<< "   dragon-parser .Main.dg.tokens" << endl
	<< "   dragon-parser -debug .Main.dg.tokens" << endl
	<< endl
	<< "GITHUB" << endl
	<< "   https://github.com/pointbazaar/dragon-parser/" << endl
	<< endl
	<< "AUTHOR" << endl
	<< "	alex23667@gmail.com" << endl
	<< endl
	<< "REPORTING BUGS" << endl
	<< "   https://github.com/pointbazaar/dragon/issues" << endl
	<< endl;
		
}

TokenList readTokensFromTokensFile(string tokensFile, bool debug){

	string fileNameWithoutPath = tokensFile;
	
	TokenList tks(tokensFile);
	ifstream file(tokensFile);
    string str; 
    while (getline(file, str)){

		optional<Token> tkn = recognizeToken(str, debug);
    	if(tkn.has_value()){
			tks.add(tkn.value());
    	}
    }

	if(debug) {
		cout << "read was successful" << endl;
		cout << "done recognizing " << tks.size() << " tokens" << endl;
	}
	return tks;
}

