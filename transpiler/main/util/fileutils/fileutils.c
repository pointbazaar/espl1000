#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <malloc.h>
#include <regex.h>

#include "fileutils.h"
#include "../../flags.h"

char* make_ast_filename(char* filename){
	
	char* ast_filename = malloc(strlen(filename)+10);
	
	char* fnamecpy = malloc(strlen(filename)+1);
	strcpy(fnamecpy, filename);
	char* base_name = basename(fnamecpy);
	
	char* fnamecpy2 = malloc(strlen(filename)+1);
	strcpy(fnamecpy2, filename);
	char* dir_name = dirname(fnamecpy2);
	
	sprintf(ast_filename, "%s/.%s.ast", dir_name, base_name);
	
	free(fnamecpy);
	free(fnamecpy2);
	
	return ast_filename;
}

char* make_c_filename(char* filename){

	char* fname_out = malloc(strlen(filename)+1);

	strcpy(fname_out, filename);
	
	//remove the '.dg'
	fname_out[strlen(fname_out)-3] = '\0';
	
	strcat(fname_out, ".c");
	
	return fname_out;
}


char* make_h_filename(char* filename){

	char* fname_out = malloc(strlen(filename)+1);

	strcpy(fname_out, filename);
	
	//remove the '.dg'
	fname_out[strlen(fname_out)-3] = '\0';
	
	strcat(fname_out, ".h");
	
	return fname_out;
}

char* make_gcc_cmd(struct Flags* flags, char* fname_out){
	
	char* cmd = malloc(100+strlen(fname_out));
	strcpy(cmd, "");
		
	strcat(cmd, "gcc -Wall --std=c11 -o a.out ");
	
	if(flags->debug_symbols){ strcat(cmd, "-g ");      }
	if(flags->werror)       { strcat(cmd, "-Werror "); }
	
	strcat(cmd, fname_out);
	
	strcat(cmd, " -lpthread -lm ");
	
	return cmd;
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
            printf("[smalldragon][Error] filename %s did not match the required pattern for smalldragon filenames\n", filename);
            return false;
        }
    }
    return true;
}