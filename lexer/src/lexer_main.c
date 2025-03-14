#define _GNU_SOURCE
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/mman.h>

#include "driver.h"
#include "lexer_flags.h"
#include "lexer_main.h"
#include "lexer.h"

struct TokenList* lexer_main(struct LexerFlags* myargs) {

	int status = 0;

	if (myargs->filename == NULL) {
		fprintf(stderr, "[Lexer] expected a filename of the file to tokenize\n");
		return NULL;
	}

	char* filename = myargs->filename;

	FILE* yyin = fopen(filename, "r");
	if (yyin == NULL) {
		fprintf(stderr, "[Lexer] error: could not open %s\n", filename);
		return NULL;
	}

	if (myargs->debug) {
		fprintf(stderr, "[Lexer] opened input file %s\n", filename);
	}

	struct TokenList* list = lexer_impl(yyin);

	if (list && myargs->write_token_file) {
		char* buffer = lexer_make_tkn_filename(filename);

		assert(buffer);

		if (myargs->debug) {
			fprintf(stderr, "[Lexer] opened output file %s\n", buffer);
		}

		int outFd = open(buffer, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
		if (outFd < 0) {
			fprintf(stderr, "[Lexer] error: could not open %s\n", buffer);
			return NULL;
		}

		list_dump_to_file(list, outFd);

		free(buffer);
	}

	fclose(yyin);

	return list;
}
