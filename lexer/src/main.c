

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

//the entire program never properly deallocates memory,
//since we assume it will not run for long, as typical source files
//are less than 1000 lines

//https://www.youtube.com/watch?v=GUQx72j9Q3Y

//standard headers
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <libgen.h>
#include <malloc.h>

//user headers
#include "tokens.h"
#include "lexer.h"
#include "loop.h"

int tokenize_file(char* filename, char* tkn_filename);

int main(int argc, char** argv) {

	//set malloc parameters
	//https://stackoverflow.com/questions/27451220/how-can-i-correctly-handle-malloc-failure-in-c-especially-when-there-is-more-th
	mallopt(M_CHECK_ACTION,3);

	/*
		writing a table based lexer
		has taught me to keep the language simple,
		and not use too many keywords.
	*/
    
    if(DEBUG){
    	printf("starting Dragon Lexer\n");
	}

    if( argc != 2 ){
    	printf("expected one argument, a filename of the file to tokenize\n");
    	return 1;
    }

    //filenames
    char* filename = argv[1];

    if(strcmp(filename,"--version")==0){
    	printf("dragon-lexer 0.8 \n");
    	exit(0);
    }else if(strcmp(filename,"--help")==0){
    	printf("Usage: dragon-lexer FILE \n");
    	printf("\n");
    	printf("Converts a .dg Source File into a .tokens file, \ncontaining the Tokens contained in the Source. \n");
    	printf("\n");

    	printf("Possible Arguments:\n");
    	printf(" --version\n");
    	printf(" --help\n\n");

    	printf("Author: \n");
    	printf("alex23667@gmail.com\n");
    	printf("\n");

    	printf("Bug Reports:\n");
    	printf("https://github.com/pointbazaar/dragon-lexer\n");
    	exit(0);
    }

    char* correct_extension = ".dg";

    //enforce correct filename extension
    if(strcmp(filename + strlen(filename) - 3,correct_extension)!=0){
    	printf("%s did not have correct '.dg' extension. Exiting \n",filename);
    	exit(1);
    }

    //because basename,dirname may modify their args
    char cpy_filename_1[50];
    char cpy_filename_2[50];
    strcpy(cpy_filename_1,filename);
    strcpy(cpy_filename_2,filename);

    if(DEBUG){
    	printf("argument file to lex: %s\n",filename);
	}

    char* dir = dirname(cpy_filename_1);
    if(DEBUG){
    	printf("in directory: %s\n",dir);
	}


	char tkn_filename[150];
	tkn_filename[0]='\0';
	strcat(tkn_filename,dir);
	strcat(tkn_filename,"/.");
	strcat(tkn_filename,basename(cpy_filename_2));
    strcat(tkn_filename,".tokens");

    if(DEBUG){
    	printf("token filename should be: %s\n",tkn_filename);
	}
    
    if( access(filename,F_OK) == -1){
		printf("could not access file \n");    
		exit(1);
    }

    //file metadata
    struct stat file_meta;
    struct stat tkn_file_meta;
    
    stat(filename,&file_meta);

	if( access(tkn_filename,F_OK) == -1){
		if(DEBUG){
			printf("could not find corresponding .tokens file \n");
		}
        tokenize_file(filename,tkn_filename);
	}else{
		//tokenized file already exists
		stat(tkn_filename,&tkn_file_meta);
		
		if(DEBUG){
			printf("tokenized file already exists, comparing modified times.\n");
		}
        
        time_t mod_file = file_meta.st_mtime;
        time_t mod_tkns = tkn_file_meta.st_mtime;
        
        if( mod_file > mod_tkns ){
            //the file was modified. the .tokens file is outdate
            if(DEBUG){
				printf("the file was modified. the tokens file is outdated.\n");
			}
            tokenize_file(filename,tkn_filename);
        }else{
			if(DEBUG){
				printf(".tokens file for this file is up to date. exiting.\n");
			}
            return 0;
        }
	}

	if(DEBUG){
		printf("SUCCESS\n");
	}
	
	return 0;
}

int tokenize_file(char* filename, char* tkn_filename){
	/*
		should tokenize  
			filename     
		and write those tokens to
			tkn_filename
	*/
	
	if(DEBUG){
		printf("opening %s to determine file length \n",filename);
	}

	FILE* file1 = fopen(filename,"r");
	
	if(file1 == NULL){
		printf("could not open file \n");
		exit(1);
	}
	
	fseek(file1,0,SEEK_END);
	long length = ftell(file1);
	fseek(file1,0,SEEK_SET);
	fclose(file1);
	
	//lex source code into tokens
	lex_main(tkn_filename,filename,length,true);

	return 0;
}
