#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <libgen.h>
#include <malloc.h>
#include <regex.h>

#include "../../../util/exit_malloc/exit_malloc.h"

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

	//generated filenames
	char* asm_filename;
	char* token_filename;
	char* hex_filename;
};


static char* make_asm_filename(char* filename){

    char* fname_out = exit_malloc(strlen(filename)+4);
    
    strcpy(fname_out, filename);
    //remove the '.dg'
    fname_out[strlen(fname_out)-3] = '\0';
    strcat(fname_out, ".asm");

    return fname_out;
}

static char* make_hex_filename(char* filename){

    char* fname_out = exit_malloc(strlen(filename)+4);
    
    strcpy(fname_out, filename);
    //remove the '.dg'
    fname_out[strlen(fname_out)-3] = '\0';
    strcat(fname_out, ".hex");

    return fname_out;
}

static char* make_token_filename(char* filename){
	
	char* token_filename = exit_malloc(strlen(filename) + 20);
	
	char* fnamecpy = exit_malloc(strlen(filename)+1);
	strcpy(fnamecpy, filename);
	char* base_name = basename(fnamecpy);
	
	char* fnamecpy2 = exit_malloc(strlen(filename)+1);
	strcpy(fnamecpy2, filename);
	char* dir_name = dirname(fnamecpy2);
	
	sprintf(token_filename, "%s/.%s.tokens", dir_name, base_name);
	
	free(fnamecpy);
	free(fnamecpy2);
	
	return token_filename;
}

static void make_flags_inner(struct Flags* flags, char* arg);

struct Flags* makeFlags(int argc, char** argv){

	struct Flags* flags = exit_malloc(sizeof(struct Flags));

	flags->debug 	= false;
	flags->help 	= false;
	flags->version 	= false;

	flags->count_filenames    = 0;
	
	//a capacity of argc guarantees that all 
	//filenames will fit
	flags->capacity_filenames = argc;
	flags->filenames = exit_malloc(sizeof(char*)*argc);
	
	for(int i=1; i < argc; i++){
		make_flags_inner(flags, argv[i]);
	}
	
	if(flags->help || flags->version){ return flags; }
	
	validate_flags(flags);
	
	flags->asm_filename   = make_asm_filename(flags_filenames(flags,0));
	flags->token_filename = make_token_filename(flags_filenames(flags,0));
	flags->hex_filename   = make_hex_filename(flags_filenames(flags,0));
	
	return flags;
}

struct Flags* makeFlagsSingleFile(char* filename){
	
	char* argv[] = {"program", filename};
	return makeFlags(2, argv);
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
	
	free(flags->asm_filename);
	free(flags->hex_filename);
	free(flags->token_filename);
	free(flags->filenames);
	free(flags);
}

int flags_count_filenames(struct Flags* flags){
	return flags->count_filenames;
}

char* flags_filenames(struct Flags* flags, int index){
	
	if(index >= flags_count_filenames(flags)){
		printf("flags_filenames: tried to access out of bounds.");
		printf("exiting.");
		fflush(stdout);
		exit(1);
	}
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

char* flags_asm_filename(struct Flags* flags){
	return flags->asm_filename;
}

char* flags_token_filename(struct Flags* flags){
	return flags->token_filename;
}

char* flags_hex_filename(struct Flags* flags){
	return flags->hex_filename;
}
