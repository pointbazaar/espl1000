#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <malloc.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#include "fileutils.h"
#include "../../flags/flags.h"

char* make_ast_filename(char* filename){
	
	char* ast_filename = malloc(100);
	
	char fnamecpy[100];
	strcpy(fnamecpy, filename);
	
	char* base_name = basename(fnamecpy);
	char* dir_name = dirname(fnamecpy);
	
	sprintf(ast_filename, "%s/.%s.ast", dir_name, base_name);
	
	return ast_filename;
}

char* make_c_filename(char* filename){

	char* fname_out = malloc(100);

	strcpy(fname_out, filename);
	
	//remove the '.dg'
	fname_out[strlen(fname_out)-3] = '\0';
	
	strcat(fname_out, ".c");
	
	return fname_out;
}


char* make_h_filename(char* filename){

	char* fname_out = malloc(100);

	strcpy(fname_out, filename);
	
	//remove the '.dg'
	fname_out[strlen(fname_out)-3] = '\0';
	
	strcat(fname_out, ".h");
	
	return fname_out;
}

char* make_gcc_cmd(struct Flags* flags, char* fname_out){
	
	char* cmd_gcc = malloc(500);
	strcpy(cmd_gcc, "");
	
	if(flags->avr){
		//choose -mmcu=attiny25, but you can replace with
		//whatever you like
		//we chase attiny25 to have it generate less complex instructions
		strcat(cmd_gcc, "avr-gcc -o main.o -I /usr/share/avra -mmcu=attiny45 ");
	}else{
		
		strcat(cmd_gcc, "gcc -Wall -o a.out ");
	}
	
	if(flags->debug_symbols){ strcat(cmd_gcc, "-g "); }
	if(flags->werror){ strcat(cmd_gcc, "-Werror "); }
	
	strcat(cmd_gcc, fname_out);
	
	strcat(cmd_gcc, " -lpthread -lm ");
	
	return cmd_gcc;
}
