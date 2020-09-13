#include <stdlib.h>
#include <stdio.h>

void* smalloc(size_t size){

	return malloc(size);
	//no branching, for performance reasons
	/*
	if(res == NULL){
		printf("malloc failed\n");
		exit(1);
	}
	*/
}
