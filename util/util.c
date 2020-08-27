#include <stdlib.h>
#include <stdio.h>

void* smalloc(size_t size){

	void* res = malloc(size);
	if(res == NULL){
		printf("malloc failed\n");
		exit(1);
	}
	return res;
}
