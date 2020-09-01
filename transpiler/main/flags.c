#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "flags.h"

struct Flags* makeFlags(int argc, char** argv){
	struct Flags* flags = malloc(sizeof(struct Flags));
	
	flags->debug = false;
	flags->avr = false;
	flags->test = false;
	flags->help = false;
	
	//if there are more than 100, it can exit
	const int gcc_flags_max = 100;
	flags->gcc_flags = malloc(sizeof(char*)*gcc_flags_max);
	flags->gcc_flags_count = 0;
	
	for(int i=1; i < argc; i++){
		char* arg = argv[i];
		if(arg[0] != '-'){
			//arg is not a flag
			continue;
		}
		
			  if(strcmp(arg, FDEBUG) == 0){ flags->debug = true;
		}else if(strcmp(arg, FTEST)  == 0){ flags->test = true;
		}else if(strcmp(arg, FAVR )  == 0){ flags->avr = true;
		}else if(strcmp(arg, FHELP )  == 0){ flags->help = true;
		}else{
			//pass this flag when calling gcc
			flags->gcc_flags[flags->gcc_flags_count] = arg;
			flags->gcc_flags_count++;
			
			if(flags->gcc_flags_count > gcc_flags_max){
				printf("Exceeded max amount");
				printf(" of gcc flags to pass through.\n");
				exit(1);
			}	
		}
	}
	
	return flags;
}

void freeFlags(struct Flags* flags){
	
	free(flags->gcc_flags);
	free(flags);
}
