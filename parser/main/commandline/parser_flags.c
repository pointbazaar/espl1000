#include <stdlib.h>
#include <string.h>

#include "parser_flags.h"

const char* FLAG_DEBUG = "-debug";
const char* FLAG_HELP  = "-help";
const char* FLAG_TEST  = "-test";

struct ParserFlags* parseFlags(int argc, char** argv){

	struct ParserFlags* flags = malloc(sizeof(struct ParserFlags));

	flags->help  = false;
	flags->test  = false;
	flags->debug = false;

	for(int i = 1; i < argc; i++){

		char* arg = argv[i];

		if(strcmp(FLAG_HELP,  arg) == 0){ flags->help  = true; }
		if(strcmp(FLAG_TEST,  arg) == 0){ flags->test  = true; }
		if(strcmp(FLAG_DEBUG, arg) == 0){ flags->debug = true; }

		if(arg[0] != '-')
			{ flags->filename = arg; }
	}
	
	return flags;
}
