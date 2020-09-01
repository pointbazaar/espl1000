#include <stdlib.h>

#include "flags.h"

struct Flags* makeFlags(){
	struct Flags* flags = malloc(sizeof(struct Flags));
	flags->debug = false;
	flags->avr = false;
	flags->test = false;
	flags->help = false;
	
	flags->gcc_flags_count = 0;
	
	return flags;
}

void freeFlags(struct Flags* flags){
	
	free(flags->gcc_flags);
	free(flags);
}
