#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "flags.h"

struct Flags* makeFlags(int argc, char** argv){
	
	struct Flags* flags = malloc(sizeof(struct Flags));
	
	flags->debug 	= false;
	flags->avr		= false;
	flags->test 	= false;
	flags->help 	= false;
	flags->version 	= false;
	flags->clean 	= false;
	
	int gcc_flags_capacity = 30; 
	
	flags->gcc_flags = malloc(sizeof(char*)*gcc_flags_capacity);
	flags->gcc_flags_count = 0;
	
	for(int i=1; i < argc; i++){
		
		char* arg = argv[i];
		
		if(arg[0] != '-'){
			//arg is not a flag
			continue;
		}
		
			  if(strcmp(arg, "-debug"  ) == 0){ flags->debug = true;
		}else if(strcmp(arg, "-test"   ) == 0){ flags->test = true;
		}else if(strcmp(arg, "-avr"    ) == 0){ flags->avr = true;
		}else if(strcmp(arg, "-help"   ) == 0){ flags->help = true;
		}else if(strcmp(arg, "-version") == 0){ flags->version = true;
		}else if(strcmp(arg, "-clean"  ) == 0){ flags->clean = true;
		}else{
			
			flags->gcc_flags[flags->gcc_flags_count] = arg;
			flags->gcc_flags_count++;
			
			if(flags->gcc_flags_count >= gcc_flags_capacity){
				
				printf("Exceeded gcc_flags_capacity!. Exiting.\n");
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
