#include "lexer/src/lexer_flags.h"
#define _XOPEN_SOURCE

#include <stdbool.h>
#define _GNU_SOURCE 1
#include <libgen.h>

int asprintf(char** restrict strp, const char* restrict fmt, ...);

#include "invoke.h"
#include "parser/main/util/parser.h"

#include "lexer/src/lexer_main.h"

struct TokenList* invoke_lexer(char* filename, bool write_token_file, bool debug) {

	struct LexerFlags flags;
	flags.write_token_file = write_token_file;
	flags.filename = filename;
	flags.debug = debug;
	return lexer_main(&flags);
}

struct Namespace* invoke_parser(struct TokenList* list, char* filename_display) {

	struct Namespace* ns = build_namespace(list, filename_display);

	return ns;
}
