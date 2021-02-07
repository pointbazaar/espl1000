// https://nothings.org/computer/lexing.html
//
// this is supposed to be a lexer project for the dragon programming language
//
// it should read a .dg file and save the tokens to a .dg.tokens file.
// it should provide the line number as part of the token
// it should encode properties of tokens as strings of numbers, which makes them easier to parse
// later on.
//
// it should receive a single filename as argument, the file to be lexed.
//
// it should work incrementally. if it detects time_modified(file.dg) <
// time_modified(file.dg.tokens)
// then it should not do anything.
//
// lexing multiple files in parallel would be supported by starting multiple instances of this
// lexer.

//https://www.youtube.com/watch?v=GUQx72j9Q3Y

//writing a table based lexer has taught me to keep 
//the language simple, and not use too many keywords.

//standard headers
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <libgen.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>

//user headers
#include "../../token/token.h"
#include "lexer.h"
#include "loop.h"
#include "test.h"
#include "lexer_flags.h"
// ---------------------

int tokenize_file(char* filename, char* tkn_filename, bool debug);

void lexer_print_help();

struct LexerFlags* handleArguments(int argc, char** argv);

int lexer_main_inner(struct LexerFlags* flags, char* tkn_filename);

void lexer_check_extension(char* filename);

char* lexer_make_tkn_filename(char* filename, bool debug);

int main(int argc, char** argv) {

	//set malloc parameters
	//https://stackoverflow.com/questions/27451220/how-can-i-correctly-handle-malloc-failure-in-c-especially-when-there-is-more-th
	mallopt(M_CHECK_ACTION,3);

	struct LexerFlags* flags = handleArguments(argc, argv);
	
	if(flags->debug) { 
		printf("--- dragon-lexer ---\n"); 
		printf("file to tokenize: %s\n",flags->filename);
	}

	lexer_check_extension(flags->filename);

	char* tkn_filename = lexer_make_tkn_filename(flags->filename, flags->debug);

	if(flags->debug) {
		printf("token filename should be: %s\n",tkn_filename);
	}

	if( access(flags->filename,F_OK) == -1) {
		printf("could not access file \n");
		free(tkn_filename);
		exit(1);
	}

	int status = lexer_main_inner(flags, tkn_filename);
	
	free(tkn_filename);
	free(flags);
	
	return status;
}

int lexer_main_inner(struct LexerFlags* flags, char* tkn_filename){
	
	char* filename = flags->filename;
	
	if(flags->clean){	
		tokenize_file(filename, tkn_filename, flags->debug);
		return 0;
	}
	
	//file metadata
	struct stat file_meta;
	struct stat tkn_file_meta;

	stat(filename,&file_meta);

	if( access(tkn_filename,F_OK) == -1) {
		if(flags->debug) {
			printf("could not find corresponding .tokens file \n");
		}
		tokenize_file(filename, tkn_filename, flags->debug);
	} else {
		//tokenized file already exists
		stat(tkn_filename,&tkn_file_meta);

		if(flags->debug) {
			printf("tokenized file already exists, comparing modified times.\n");
		}

		size_t mod_file = file_meta.st_mtime;
		size_t mod_tkns = tkn_file_meta.st_mtime;

		if( mod_file > mod_tkns ) {
			//the file was modified. the .tokens file is outdate
			if(flags->debug) {
				printf("the file was modified. the tokens file is outdated.\n");
			}
			tokenize_file(filename, tkn_filename, flags->debug);
		} else {
			if(flags->debug) {
				printf(".tokens file for this file is up to date. exiting.\n");
			}
			return 0;
		}
	}
	
	return 0;
}

int tokenize_file(char* filename, char* tkn_filename, bool debug) {

	//should tokenize filename
	//and write those tokens to	tkn_filename

	if(debug) {
		printf("opening %s to determine file length \n",filename);
	}

	FILE* file1 = fopen(filename,"r");

	if(file1 == NULL) {
		printf("could not open file \n");
		exit(1);
	}

	fseek(file1,0,SEEK_END);
	long length = ftell(file1);
	fseek(file1,0,SEEK_SET);
	fclose(file1);

	//lex source code into tokens
	lex_main(tkn_filename,filename,length,true, debug);

	return 0;
}

struct LexerFlags* handleArguments(int argc, char** argv){
	//this subroutine may perform an exit(...)
	//this is ok because it is called in main(...)
	//before anything is allocated on the heap
	struct LexerFlags* res = malloc(sizeof(struct LexerFlags));
	
	res->filename = NULL;
	res->debug = false;
	res->clean = false;
	
	for(int i=1; i < argc; i++) {

		char* arg = argv[i];
		if(arg[0] == '-') {
			if(strcmp(arg, "-debug") == 0) {
				
				res->debug = true;
				
			} else if(strcmp(arg, "-test") == 0) {
				
				const int status = test_all(res->debug);
				free(res);
				exit(status);	
			}
		} else {
			res->filename = arg;
		}
	}
	
	if(res->filename == NULL) {
		printf("expecte a filename of the file to tokenize\n");
		exit(1);
	}
	
	return res;
}

void lexer_check_extension(char* filename){
	
	char* correct_extension = ".dg";

	//enforce correct filename extension
	if(strcmp(filename + strlen(filename) - 3,correct_extension)!=0) {
		printf("%s did not have correct '.dg' extension. Exiting \n",filename);
		exit(1);
	}
}

char* lexer_make_tkn_filename(char* filename, bool debug){
	
	//because basename,dirname may modify their args
	char cpy_filename_1[50];
	char cpy_filename_2[50];
	strcpy(cpy_filename_1,filename);
	strcpy(cpy_filename_2,filename);

	char* dir = dirname(cpy_filename_1);
	if(debug) {
		printf("in directory: %s\n",dir);
	}

	char* tkn_filename = malloc(sizeof(char)* 150);
	
	tkn_filename[0]='\0';
	strcat(tkn_filename,dir);
	strcat(tkn_filename,"/.");
	strcat(tkn_filename,basename(cpy_filename_2));
	strcat(tkn_filename,".tokens");
	
	return tkn_filename;
}
