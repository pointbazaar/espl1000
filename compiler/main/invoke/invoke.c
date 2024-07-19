#define _XOPEN_SOURCE

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include "../../../util/exit_malloc/exit_malloc.h"
#include "util/fileutils/fileutils.h"
#include "invoke.h"
#include "parser/main/util/parser.h"

int invoke_lexer(char* filename) {

	char* cmd1 = exit_malloc(strlen(filename) + 100);

	strcpy(cmd1, "dragon-lexer ");

	strcat(cmd1, filename);

	int status = system(cmd1);
	free(cmd1);
	return status;
}

struct Namespace* invoke_parser(char* filename) {

	char* fnamecpy = exit_malloc(strlen(filename) + 1);

	strcpy(fnamecpy, filename);

	char* base_name = basename(fnamecpy);
	char* dir_name = dirname(fnamecpy);

	char* cmd2 = exit_malloc(strlen(filename) + 100);

	sprintf(cmd2, "%s/.%s.tokens", dir_name, base_name);

	struct Namespace* ns = build_namespace(cmd2);

	free(cmd2);
	free(fnamecpy);

	return ns;
}
