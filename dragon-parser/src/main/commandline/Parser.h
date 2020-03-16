#ifndef PARSER 
#define PARSER

#include <stdio.h>
#include <string.h>

#include "TokenList.h"
#include "../parsing/Namespace.h"
#include "../parsing/Method.h"
#include "../parsing/DeclArg.h"
#include "../parsing/statements/Stmt.h"

using namespace std;

const char* FLAG_DEBUG = "-debug";
const char* FLAG_HELP = "-help";
const char* FLAG_TEST = "-test";

int main(int argc, char** argv);

void build_ast_file(char* tokensFile, char* astJsonFile, bool debug);


void main_inner(char* tokensFile, bool debug);

struct TokenList* readTokensFromTokensFile(char* tokensFile, bool debug);

void printHelp();


#endif