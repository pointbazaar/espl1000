#ifndef PARSER_FLAGS_H
#define PARSER_FLAGS_H

#include <stdbool.h>

struct ParserFlags {
	bool help;
	char* filename;
};

struct ParserFlags* parseFlags(int argc, char** argv);

#endif
