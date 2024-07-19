#include <stdlib.h>
#include <string.h>

#include "../../../util/exit_malloc/exit_malloc.h"

#include "parser_flags.h"

const char* FLAG_HELP = "-help";

struct ParserFlags* parseFlags(int argc, char** argv) {

	struct ParserFlags* flags = exit_malloc(sizeof(struct ParserFlags));

	flags->help = false;

	for (int i = 1; i < argc; i++) {

		char* arg = argv[i];

		flags->help = strcmp(FLAG_HELP, arg) == 0;

		if (arg[0] != '-') { flags->filename = arg; }
	}

	return flags;
}
