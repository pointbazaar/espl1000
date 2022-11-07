#include <stdlib.h>
#include <stdio.h>

#include "exit_malloc.h"

void* exit_malloc(size_t nbytes){
	
	void* res = malloc(nbytes);
	
	if(res == NULL){
		fprintf(stderr,"could not allocate %ld bytes. exiting.", nbytes);
		fflush(stderr);
		exit(1);
	}
	
	return res;
}
