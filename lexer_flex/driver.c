#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "driver.h"
#include "lexer_flags.h"

void out(int id, char* str){
	fprintf(outFile, "%d %s\n", id, str);
}

void out2(int id, int id2){
	fprintf(outFile, "%d %d\n", id, id2);
}

void lexer_print_help(){
	printf("Usage: dragon-lexer FILE \n");
	printf("\n");
	printf("Converts a .dg Source File into a .tokens file, \ncontaining the Tokens contained in the Source. \n");
	printf("\n");

	printf("Possible Arguments:\n");
	printf(" -version\n");
	printf(" -debug\n");
	printf(" -test\n");
	printf(" -clean\n");
	printf(" -help\n\n");

	printf("Author: \n");
	printf("alex23667@gmail.com\n");
	printf("\n");

	printf("Bug Reports: alex23667@gmail.com\n");
}

struct LexerFlags* handleArguments(int argc, char** argv){
	//this subroutine may perform an exit(...)
	//this is ok because it is called in main(...)
	//before anything is allocated on the heap
	struct LexerFlags* res = malloc(sizeof(struct LexerFlags));
	
	res->filename = NULL;
	
	res->debug = false;
	res->clean = false;
	res->test  = false;
	
	for(int i=1; i < argc; i++) {

		char* arg = argv[i];
		if(arg[0] == '-') {
			if(strcmp(arg, "-debug") == 0) {
				
				res->debug = true;
				
			} else if(strcmp(arg, "-test") == 0) {
				res->test = true;
				
			} else if(strcmp(arg, "-clean") == 0) {
				
				res->clean = true;
				
			} else if(strcmp(arg, "-version") == 0) {
				
				printf("dragon-lexer 0.8 \n");
				free(res);
				exit(0);
				
			} else if(strcmp(arg, "-help") == 0) {
				
				lexer_print_help();
				free(res);
				exit(0);
			}
		} else {
			res->filename = arg;
		}
	}
	
	return res;
}
