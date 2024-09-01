#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "driver.h"
#include "lexer_flags.h"
#include "lex.yy.h"

#include "lexer_main.h"

int lexer_main(int argc, char* argv[]) {

	struct LexerFlags* myargs = handle_arguments(argc, argv);

	if (myargs->help) {
		lexer_print_help();
	}

	if (myargs->filename == NULL) {
		printf("expected a filename of the file to tokenize\n");
		exit(1);
	}

	char* filename = myargs->filename;

	//configure input source
	yyin = fopen(filename, "r");
	if (yyin == NULL) {
		fprintf(stderr, "error: could not open %s\n", filename);
		return 1;
	}

	char* buffer = lexer_make_tkn_filename(filename);

	//configure output file, NECESSARY (for FLEX)
	outFile = fopen(buffer, "w");
	if (outFile == NULL) {
		fprintf(stderr, "error: could not open %s\n", buffer);
		return 1;
	}

	//full buffering for better performance
	setvbuf(outFile, NULL, _IOFBF, BUFSIZ);

	yylex();

	fclose(yyin);
	free(myargs);

	fclose(outFile);
	yylex_destroy();
	free(buffer);

	return 0;
}
