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

//project includes
#include "Parser.hpp"

using namespace std;

int main(int argc, char** argv){
	//this project is to parse a Dragon AST
	//from Tokens written into .tokens files by Dragon-Lexer,
	//and store it in a .json file to be retrieved by the Dragon Compiler
	//for simplicity, we invoke with just one filename as argument.
	//such we can easily have
	//multiple parallel invocations of the parser in the compiler.

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
		exit(1);
	}
}

void build_ast_json_file(string tokensFile, string astJsonFile, bool debug) {
	if(debug){
		cout << "Parser::build_ast_json_file" << endl;
	}

	TokenList tokens = readTokensFromTokensFile(tokensFile,debug);
	//get just the namespace name from .FILENAME.dg.tokens
	string tokenFileName = tokensFile;
	string namespaceName = tokenFileName.substr(1,string(tokenFileName).size() - string(".dg.tokens").size());

	NamespaceNode mynamespace = NamespaceNode(tokens,namespaceName,debug);

	//TODO: generate our custom AST Format
	string str = "TODO: generate our custom AST format";

	if(debug){
		cout << "write to "+astJsonFile << endl;
	}

	ofstream file;
	file.open(astJsonFile, ios::out);
	file << str;
	file.close();
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

	//it should receive a .filename.dg.tokens file as path
	//and write a .filename.dg.json file to disk

	//in this approach it should be incremental. if the modified time
	//of the .json file is later than that of the .tokens file, the .json
	//file should not be rebuilt

	ifstream f(tokensFile.c_str());

	if(f.good()) {
		f.close();

		//TODO: re-enable the incremental later on
		//long tokensLastModified = tokensFile.lastModified();

		string astJSONFilename = tokensFile.substr(0,tokensFile.size()-string(".tokens").size())+".json";

		/*
		ifstream f2(astJSONFilename);
		if(f2.good()){
			f2.close()
			if(debug){
				cout << ast_json_file+"  already exists." << endl;
			}
			//see which is more recent
			long ast_json_last_modified = ast_json_file.lastModified();

			if(ast_json_last_modified < tokensLastModified){
				build_ast_json_file(tokensFile,ast_json_file,debug);
			}else{
				if(debug) {
					cout << "the AST .json file was more recent than the tokens file. nothing needs to be done." << endl;
				}
			}
		}else{
		*/
			//ast .json file does not exist
			build_ast_json_file(tokensFile,astJSONFilename,debug);
		//}
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

TokenList makeTokenListByCallingLexer(string file, bool debug) {

	// - call dragon-lexer with the filename
	// - read its output file, the tokens, put them in a TokenList

	string call = ((debug)?"dragon-lexer-debug ":"dragon-lexer ") + file;

	if (debug) {
		cout << call << endl;
	}

	//TODO: think about this later, 
	//when it compiles again

	/*
	Process proc = Runtime.getRuntime().exec(call);
	proc.waitFor();

	if (debug) {
		cout << "stdout: " << endl;
		BufferedReader stdInput = new BufferedReader(new InputStreamReader(proc.getInputStream()));
		string s = null;
		while ((s = stdInput.readLine()) != null) {
			cout << s << endl;
		}
	}

	if (debug) {
		cout << "stderr: " << endl;
		BufferedReader stdError = new BufferedReader(new InputStreamReader(proc.getErrorStream()));
		string s2 = null;
		while ((s2 = stdError.readLine()) != null) {
			cout << s2 << endl;
		}
	}

	if (proc.exitValue() != 0) {
		throw ("dragon-lexer exit with nonzero exit code: " + proc.exitValue());
	}


	string dirname = file.getParent();
	if (dirname == null) {
		dirname = ".";
	}
	string path = (dirname + "/." + file + ".tokens");
	if(debug) {
		cout << "read from: " + path << endl;
	}
	*/
	return readTokensFromTokensFile(file,debug);
}

TokenList readTokensFromTokensFile(string tokensFile, bool debug){

	//TODO: fix
	string fileNameWithoutPath = tokensFile;//.getName();

	//TODO: re-enable
	/*
	if(! fileNameWithoutPath.endsWith(".tokens")){
		throw ("tokens file must end with '.tokens' . Got "+fileNameWithoutPath );
	}

	if(! fileNameWithoutPath.startsWith(".")){
		throw "tokens file must start with '.'. A token File should be hidden. Got "+fileNameWithoutPath ;
	}
	*/

	vector<string> lines;
	ifstream file(tokensFile);
    string str; 
    while (getline(file, str))
    {
        lines.push_back(str);
    }

	if(debug) {
		cout << "read was successful" << endl;
	}

	TokenList tks = TokenList(tokensFile);

	for (string line : lines) {
		BaseToken tkn = TokenReader.recognizeToken(line, debug);
		tks.add(tkn);
	}

	//DEBUG
	if (debug) {
		cout << "done recognizing tokens" << endl;
	}

	return tks;
}

TokenList makeTokenList(string code,bool debug) {

	//makes token list from a code
	//randomize the file somewhat, so that the tests can be run in parallel.
	//this will fill up the disk, but it is in /tmp
	//so it will be deleted on restart?

	int rand = rand();
	string path = ((string)"/tmp/temp") + to_string(rand) + (string)(".dg");

	if(debug) {
		cout << "write to: " << path << endl;
	}

	ofstream outfile;
	outfile.open(path, ios::out);
	outfile << code;
	outfile.close();

	return makeTokenListByCallingLexer(path, debug);
}

