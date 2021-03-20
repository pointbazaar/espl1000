#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "flags.h"

#include "validate_flags.h"

const int gcc_flags_capacity = 30; 

static void make_flags_inner(struct Flags* flags, char* arg);

struct Flags* makeFlags(int argc, char** argv){
	
	struct Flags* flags = malloc(sizeof(struct Flags));
	
	flags->debug 	= false;
	flags->avr		= false;
	flags->test 	= false;
	flags->help 	= false;
	flags->version 	= false;
	flags->clean 	= false;
	flags->no_typecheck = false;
	
	flags->count_filenames = 0;
	flags->capacity_filenames = 100;
	flags->filenames = malloc(sizeof(char*)*100);
	
	flags->gcc_flags = malloc(sizeof(char*)*gcc_flags_capacity);
	flags->gcc_flags_count = 0;
	
	for(int i=1; i < argc; i++){
		
		char* arg = argv[i];
		
		make_flags_inner(flags, arg);
	}
	
	if(argc == 0){ return flags; }
	
	validate_flags(flags);
	
	return flags;
}

static void make_flags_inner(struct Flags* flags, char* arg){

	if(arg[0] != '-'){
		
		flags->filenames[flags->count_filenames] = arg;
		flags->count_filenames += 1;
		
		return;
	}
	
	if(strcmp(arg, "-debug") == 0){ 
		flags->debug = true;
		return;
	}
	
	if(strcmp(arg, "-test") == 0){ 
		flags->test = true;
		return;
	}
	
	if(strcmp(arg, "-avr") == 0){ 
		flags->avr = true;
		return;
	}
	
	if(strcmp(arg, "-help") == 0){ 
		flags->help = true;
		return;
	}
	
	if(strcmp(arg, "-version") == 0){ 
		flags->version = true;
		return;
	}
	
	if(strcmp(arg, "-clean") == 0){ 
		flags->clean = true;
		return;
	}
	
	if(strcmp(arg, "-no_typecheck") == 0){
		flags->no_typecheck = true;
		return;
	}
		
	flags->gcc_flags[flags->gcc_flags_count] = arg;
	flags->gcc_flags_count++;
	
	if(flags->gcc_flags_count >= gcc_flags_capacity){
		
		printf("Exceeded gcc_flags_capacity!. Exiting.\n");
		exit(1);
	}	

}

void freeFlags(struct Flags* flags){
	
	free(flags->filenames);
	free(flags->gcc_flags);
	free(flags);
}
