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

	const vector<string> options = createOptions();
	const vector<string> flags;
	const vector<string> filenames;

	for(int i=1;i<argc;i++){
		string arg = to_string(argv[i]);
		if(argv[i][0] == '-'){
			options.push(arg);
		}else{
			filenames.push(arg);
		}
	}

	bool debug = true;
	try {
		debug = flags.contains(FLAG_DEBUG);
		if(debug){
			cout << "Parser::main" << endl;
		}
		bool help = flags.contains(FLAG_HELP);
		bool test = flags.contains(FLAG_TEST);

		if(help) {
			ParserCLIUtils.printHelp();
		}else if(test){
			ParserTest.test_all();
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

	const TokenList tokens = ParserPhases.readTokensFromTokensFile(tokensFile,debug);
	//get just the namespace name from .FILENAME.dg.tokens
	const string tokenFileName = tokensFile.getName();
	const string namespaceName = tokenFileName.substring(1,tokenFileName.length()-(".dg.tokens").length());

	const NamespaceNode mynamespace = new NamespaceNode(tokens,namespaceName,debug);

	const string str = mapper.writeValueAsString(mynamespace);

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

	if(!tokensFile.toString().endsWith(".tokens")){
		throw (tokensFile.toString()+" does not have .tokens extension. Exiting.");
	}

	//it should receive a .filename.dg.tokens file as path
	//and write a .filename.dg.json file to disk

	//in this approach it should be incremental. if the modified time
	//of the .json file is later than that of the .tokens file, the .json
	//file should not be rebuilt

	if(tokensFile.exists()) {
		long tokensLastModified = tokensFile.lastModified();

		string astJSONFilename = tokensFile.toString().substring(0,tokensFile.toString().length()-(".tokens").length())+".json";

		File ast_json_file = (
				Paths.get(astJSONFilename)
		).toFile();

		if(ast_json_file.exists()){
			if(debug){
				out.println(ast_json_file+"  already exists.");
			}
			//see which is more recent
			long ast_json_last_modified = ast_json_file.lastModified();

			if(ast_json_last_modified < tokensLastModified){
				build_ast_json_file(tokensFile,ast_json_file,debug);
			}else{
				if(debug) {
					out.println("the AST .json file was more recent than the tokens file. nothing needs to be done.");
				}
			}
		}else{
			//ast .json file does not exist
			build_ast_json_file(tokensFile,ast_json_file,debug);
		}
	}else {
		throw ("argument file "+tokensFile.toString()+" does not exist.");
	}

}

void printHelp() {

	string header = "dragon-parser - a parser for the dragon programming language\n";

	string footer = "EXAMPLES\n" +
	                      "   dragon-parser .Main.dg.tokens\n" +
	                      "   dragon-parser -debug .Main.dg.tokens\n" +
	                      "\n" +
	                      "            GITHUB\n" +
	                      "   https://github.com/pointbazaar/dragon-parser/\n" +
	                      "\n" +
	                      "AUTHOR\n" +
	                      "alex23667@gmail.com\n" +
	                      "\n" +
	                      "            REPORTING BUGS\n" +
	                      "   https://github.com/pointbazaar/dragon/issues\n" +
	                      "\n";

	cout 
		<< "dragon-parser FILE" << endl
		<< header << endl
		<< endl
		<< String.join("\n", createOptions())
		<< endl
		<< footer << endl;
}

vector<string> createOptions() {
	vector<string> res;

	res.push_back(Parser.FLAG_DEBUG); //,false,"prints debug output"));
	res.push_back(Parser.FLAG_HELP); //false,"print help"));
	res.push_back(Parser.FLAG_TEST); //false, "run tests"));

	return res;
}

TokenList makeTokenListByCallingLexer(string file, bool debug) {

	// - call dragon-lexer with the filename
	// - read its output file, the tokens, put them in a TokenList

	string call = ((debug)?"dragon-lexer-debug ":"dragon-lexer ") + file;

	if (debug) {
		cout << call << endl;
	}

	Process proc = Runtime.getRuntime().exec(call);
	proc.waitFor();

	if (debug) {
		cout << "stdout: " << endl;
		BufferedReader stdInput = new BufferedReader(new InputStreamReader(proc.getInputStream()));
		string s = null;
		while ((s = stdInput.readLine()) != null) {
			out.println(s);
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
	return readTokensFromTokensFile(path.toFile(),debug);
}

TokenList readTokensFromTokensFile(string tokensFile, bool debug){

	string fileNameWithoutPath = tokensFile.getName();

	if(! fileNameWithoutPath.endsWith(".tokens")){
		throw ("tokens file must end with '.tokens' . Got "+fileNameWithoutPath );
	}

	if(! fileNameWithoutPath.startsWith(".")){
		throw "tokens file must start with '.'. A token File should be hidden. Got "+fileNameWithoutPath ;
	}

	vector<string> lines = Files.readAllLines(tokensFile);

	if(debug) {
		cout << "read was successful" << endl;
	}

	TokenList tks = new TokenList(tokensFile);

	for (string line : lines) {
		BaseToken tkn = TokenReader.recognizeToken(line, debug);
		tkn.ifPresent(tks::add);
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

