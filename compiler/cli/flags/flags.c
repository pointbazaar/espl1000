#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "flags.h"

#include "validate_flags.h"

struct Flags {
	//struct Flags should be opaque outside its
	//implementation files
	
	bool debug;         //-debug
	bool help;          //-help
	bool version;       //-version

    bool has_main_fn;

    uint8_t count_filenames;
    uint8_t capacity_filenames;
    char** filenames;

};

static void make_flags_inner(struct Flags* flags, char* arg);

struct Flags* makeFlags(int argc, char** argv){

	struct Flags* flags = malloc(sizeof(struct Flags));

	flags->debug 	= false;
	flags->help 	= false;
	flags->version 	= false;

	flags->count_filenames    = 0;
	
	//a capacity of argc guarantees that all 
	//filenames will fit
	flags->capacity_filenames = argc;
	flags->filenames = malloc(sizeof(char*)*argc);
	
	for(int i=1; i < argc; i++){
		make_flags_inner(flags, argv[i]);
	}
	
	if(flags->help || flags->version){ return flags; }
	
	validate_flags(flags);
	
	return flags;
}

struct Flags* makeFlags2(){

    struct Flags* flags = malloc(sizeof(struct Flags));

    flags->debug         = false;
    flags->help          = false;
    flags->version       = false;

    flags->count_filenames    = 0;
    flags->capacity_filenames = 100;
    flags->filenames = malloc(sizeof(char*)*100);

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
	
	if(strcmp(arg, "-help") == 0){ 
		flags->help = true;
		return;
	}
	
	if(strcmp(arg, "-version") == 0){ 
		flags->version = true;
		return;
	}
	
	printf("unrecognized flag: %s. Exiting.\n", arg);
	exit(1);
}

void freeFlags(struct Flags* flags){
	
	free(flags->filenames);
	free(flags);
}

int flags_count_filenames(struct Flags* flags){
	return flags->count_filenames;
}

char* flags_filenames(struct Flags* flags, int index){
	return flags->filenames[index];
}

bool flags_debug(struct Flags* flags){
	return flags->debug;
}
bool flags_version(struct Flags* flags){
	return flags->version;
}
bool flags_help(struct Flags* flags){
	return flags->help;
}
