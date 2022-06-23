#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <malloc.h>
#include <regex.h>

#include "fileutils.h"

char* make_token_filename(char* filename){
	
	char* token_filename = malloc(strlen(filename) + 10);
	
	char* fnamecpy = malloc(strlen(filename)+1);
	strcpy(fnamecpy, filename);
	char* base_name = basename(fnamecpy);
	
	char* fnamecpy2 = malloc(strlen(filename)+1);
	strcpy(fnamecpy2, filename);
	char* dir_name = dirname(fnamecpy2);
	
	sprintf(token_filename, "%s/.%s.tokens", dir_name, base_name);
	
	free(fnamecpy);
	free(fnamecpy2);
	
	return token_filename;
}


char* make_h_filename(char* filename){

	char* fname_out = malloc(strlen(filename)+1);

	strcpy(fname_out, filename);
	
	//remove the '.dg'
	fname_out[strlen(fname_out)-3] = '\0';
	
	strcat(fname_out, ".h");
	
	return fname_out;
}

char* make_asm_filename(char* filename){

    char* fname_out = malloc(strlen(filename)+1);
    strcpy(fname_out, filename);
    //remove the '.dg'
    fname_out[strlen(fname_out)-3] = '\0';
    strcat(fname_out, ".asm");

    return fname_out;
}


bool check_filenames_lowercase(struct Flags* flags){

    char* filename_pattern = "^[a-z\\/._0-9]+$";
    regex_t regex;
    int status = regcomp(&regex, filename_pattern, REG_EXTENDED);

    if(status != 0){
        perror("error:");
        return false;
    }

    for (int i = 0; i < flags->count_filenames; ++i) {
        char* filename = flags->filenames[i];
        //abc
        //find only the filename without the path
        char* filename_only = filename + strlen(filename) - 1;

        while(filename_only[0] != '/' && filename < filename_only){
            filename_only--;
        }
        filename_only++;

        int result = regexec(&regex,filename_only, 0, NULL, 0);

        if(result != 0){
            printf("[Error] filename %s did not match the required pattern for filenames\n", filename);
            return false;
        }
    }

	regfree(&regex);

    return true;
}