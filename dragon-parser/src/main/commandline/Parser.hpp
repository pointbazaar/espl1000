#ifndef PARSER 
#define PARSER

//standard includes
#include <vector> 
#include <stdio.h>
#include <map>
#include <algorithm>
#include <optional>

//project includes
#include "../compiler/lexing/utils/TokenList.hpp"
#include "../compiler/parsing/astnodes/nonterminal/upperscopes/NamespaceNode.hpp"

using namespace std;

const string FLAG_DEBUG = "-debug";
const string FLAG_HELP = "-help";
const string FLAG_TEST = "-test";

int main(int argc, char** argv);

void build_ast_json_file(File tokensFile, File astJsonFile, bool debug) throws Exception;

void main_inner(File tokensFile, bool debug) throws Exception;

TokenList makeTokenListByCallingLexer(File file, bool debug) throws Exception;
TokenList readTokensFromTokensFile(File tokensFile, bool debug)throws Exception;
TokenList makeTokenList(string code,bool debug) throws Exception;

vector<string> createOptions();
void printHelp();

#endif