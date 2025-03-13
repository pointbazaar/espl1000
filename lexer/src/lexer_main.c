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

int lexer_main(struct LexerFlags* myargs) {

	int status = 0;
	bool debug = false;

	if (myargs->filename == NULL) {
		fprintf(stderr, "[Lexer] expected a filename of the file to tokenize\n");
		return -1;
	}

	char* filename = myargs->filename;

	FILE* yyin = fopen(filename, "r");
	if (yyin == NULL) {
		fprintf(stderr, "[Lexer] error: could not open %s\n", filename);
		return -1;
	}

	if (debug) {
		fprintf(stderr, "[Lexer] opened input file %s\n", filename);
	}

	char* buffer = lexer_make_tkn_filename(filename);

	assert(buffer);

	int outFd;

	if (myargs->write_token_file) {
		outFd = open(buffer, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
		if (outFd < 0) {
			fprintf(stderr, "[Lexer] error: could not open %s\n", buffer);
			return -1;
		}
	} else {
		outFd = memfd_create((const char*)buffer, 0);

		if (outFd < 0) {
			fprintf(stderr, "[Lexer] error: could not create memfd for %s\n", buffer);
			return -1;
		}
	}

	if (debug) {
		fprintf(stderr, "[Lexer] opened output file %s\n", buffer);
	}

	status = lexer_impl(yyin, outFd);

	fclose(yyin);

	free(buffer);

	return outFd;
}
