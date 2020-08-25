#include <stdlib.h>

#include "flags.h"

struct Flags* makeFlags(){
	struct Flags* flags = malloc(sizeof(struct Flags));
	flags->debug = false;
	flags->avr = false;
	flags->test = false;
	
	//flags->gcc_flags = 
	flags->gcc_flags_count = 0;
	
	return flags;
}

void freeFlags(struct Flags* flags){
	
	//flags->gcc_flags_count;
	free(flags->gcc_flags);
	free(flags);
}
