#include "lexer/src/lexer_flags.h"
#define _XOPEN_SOURCE

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define _GNU_SOURCE 1
#include <stdio.h>
#include <libgen.h>

int asprintf(char** restrict strp, const char* restrict fmt, ...);

#include "invoke.h"
#include "parser/main/util/parser.h"

#include "lexer/src/lexer_main.h"

int invoke_lexer(char* filename, bool write_token_file) {

	struct LexerFlags flags;
	flags.write_token_file = write_token_file;
	flags.filename = filename;
	return lexer_main(&flags);
}

struct Namespace* invoke_parser(int tokensFd, char* filename_display) {

	struct Namespace* ns = build_namespace(tokensFd, filename_display);

	return ns;
}
