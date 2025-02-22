#define _XOPEN_SOURCE

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define _GNU_SOURCE 1
#include <stdio.h>
#include <libgen.h>

int asprintf(char** restrict strp, const char* restrict fmt, ...);

#include "util/fileutils/fileutils.h"
#include "invoke.h"
#include "parser/main/util/parser.h"

#include "lexer/src/lexer_main.h"

int invoke_lexer(char* filename) {

	char* argv[] = {"programname", filename};
	int status = lexer_main(2, argv);

	return status;
}

struct Namespace* invoke_parser(char* filename) {

	char* fnamecpy = malloc(strlen(filename) + 1);

	if (!fnamecpy) {
		return NULL;
	}

	strcpy(fnamecpy, filename);

	char* base_name = basename(fnamecpy);
	char* dir_name = dirname(fnamecpy);

	char* cmd2;
	asprintf(&cmd2, "%s/.%s.tokens", dir_name, base_name);

	struct Namespace* ns = build_namespace(cmd2);

	free(cmd2);
	free(fnamecpy);

	return ns;
}
