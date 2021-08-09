#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "driver.h"
#include "lexer_flags.h"
#include "../lex.yy.h"

#include "../../token/TokenKeys.h" 

/*
extern void yylex();
extern void yylex_destroy();
extern FILE* yyin;
*/
/*
FILE* yyin;
void yylex();
void yylex_destroy();
*/

int main(int argc, char* argv[]){
	
	struct LexerFlags* myargs = handle_arguments(argc, argv);
	
	if(myargs->help){
		lexer_print_help();
	}
	
	if(myargs->version){
		printf("dragon-lexer v0.8.1\n");
		free(myargs);
		exit(0);
	}
	
	if(myargs->filename == NULL) {
		printf("expecte a filename of the file to tokenize\n");
		exit(1);
	}
	
	char* filename = myargs->filename;
	
	//configure input source
	yyin = fopen(filename, "r");
	
	char* buffer = lexer_make_tkn_filename(filename);
	
	//configure output file, NECESSARY (for FLEX)
	outFile = fopen(buffer, "w");
	
	//full buffering for better performance
	setvbuf(outFile, NULL, _IOFBF, BUFSIZ);
	
	yylex();
	
	fclose(yyin);
	free(myargs);
	
	fclose(outFile);
	yylex_destroy();
	free(buffer);
}
