#ifndef PARSER 
#define PARSER

//standard includes
#include <vector> 
#include <stdio.h>
#include <map>
#include <algorithm>
#include <optional>
#include <string>
#include <iostream>
#include <fstream>

//project includes
#include "TokenList.hpp"
#include "../parsing/NamespaceNode.hpp"
#include "../parsing/MethodNode.hpp"
#include "../parsing/DeclaredArgumentNode.hpp"
#include "../parsing/statements/StatementNode.hpp"

using namespace std;

const string FLAG_DEBUG = "-debug";
const string FLAG_HELP = "-help";
const string FLAG_TEST = "-test";

int main(int argc, char** argv);

void build_ast_json_file(string tokensFile, string astJsonFile, bool debug);

void write_ast(string filename, NamespaceNode namespaceNode);

void main_inner(string tokensFile, bool debug);

TokenList makeTokenListByCallingLexer(string file, bool debug);
TokenList readTokensFromTokensFile(string tokensFile, bool debug);
TokenList makeTokenList(string code,bool debug);

void printHelp();


void write(NamespaceNode m, ofstream* file);
void write(MethodNode m, ofstream* file);

void write(DeclaredArgumentNode m, ofstream* file);
void write(StatementNode m, ofstream* file);

void write(TypeNode m, ofstream* file);

#endif