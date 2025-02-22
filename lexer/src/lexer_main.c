#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "driver.h"
#include "lexer_flags.h"
#include "lexer_main.h"
#include "lexer.h"

int lexer_main(int argc, char* argv[]) {

	int status = 0;
	bool debug = false;
	struct LexerFlags* myargs = handle_arguments(argc, argv);

	if (myargs->filename == NULL) {
		fprintf(stderr, "[Lexer] expected a filename of the file to tokenize\n");
		return 1;
	}

	char* filename = myargs->filename;

	//configure input source
	FILE* yyin = fopen(filename, "r");
	if (yyin == NULL) {
		fprintf(stderr, "[Lexer] error: could not open %s\n", filename);
		return 1;
	}

	if (debug) {
		fprintf(stderr, "[Lexer] opened input file %s\n", filename);
	}

	char* buffer = lexer_make_tkn_filename(filename);

	FILE* outFile = fopen(buffer, "w");
	if (outFile == NULL) {
		fprintf(stderr, "[Lexer] error: could not open %s\n", buffer);
		return 1;
	}

	if (debug) {
		fprintf(stderr, "[Lexer] opened output file %s\n", buffer);
	}

	//full buffering for better performance
	setvbuf(outFile, NULL, _IOFBF, BUFSIZ);

	status = lexer_impl(yyin, outFile);

	fclose(yyin);
	free(myargs);

	fclose(outFile);
	free(buffer);

	return status;
}
