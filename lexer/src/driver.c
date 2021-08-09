#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>

#include "driver.h"
#include "lexer_flags.h"

#include "../../token/TokenKeys.h" 

FILE* outFile;

void out(int id, char* str){
	fprintf(outFile, "%d %s\n", id, str);
}

void out2(int id, int id2){
	fprintf(outFile, "%d %d\n", id, id2);
}

void out_plus_plus(){
	out(ASSIGNOP, "+=");
	out(INTEGER, "1");
}

void out_minus_minus(){
	out(ASSIGNOP, "-=");
	out(INTEGER, "1");
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
	printf(" -help\n\n");

	printf("Author: \n");
	printf("alex23667@gmail.com\n");
	printf("\n");

	printf("Bug Reports: alex23667@gmail.com\n");
}

struct LexerFlags* handle_arguments(int argc, char** argv){
	//this subroutine may perform an exit(...)
	//this is ok because it is called in main(...)
	//before anything is allocated on the heap
	struct LexerFlags* res = malloc(sizeof(struct LexerFlags));
	
	res->filename = NULL;
	res->version = false;
	res->help    = false;
	
	for(int i=1; i < argc; i++) {

		char* arg = argv[i];
		if(arg[0] == '-') {
			if(strcmp(arg, "-version") == 0) {
				
				res->version = true;
				
			} else if(strcmp(arg, "-help") == 0) {
				
				lexer_print_help();
				free(res);
				exit(0);
			} else {
				
				printf("[Lexer] unrecognized flag: %s\n", arg);
				printf("[Lexer] exiting.\n");
				exit(1);
			}
		} else {
			res->filename = arg;
		}
	}
	
	return res;
}

char* lexer_make_tkn_filename(char* filename){
	
	//because basename,dirname may modify their args
	char* cpy_filename_1 = malloc(strlen(filename)+1);
	char* cpy_filename_2 = malloc(strlen(filename)+1);
	strcpy(cpy_filename_1,filename);
	strcpy(cpy_filename_2,filename);

	char* dir = dirname(cpy_filename_1);

	char* tkn_filename = malloc(strlen(filename)+1+100);
	
	tkn_filename[0]='\0';
	strcat(tkn_filename,dir);
	strcat(tkn_filename,"/.");
	strcat(tkn_filename,basename(cpy_filename_2));
	strcat(tkn_filename,".tokens");
	
	return tkn_filename;
}

