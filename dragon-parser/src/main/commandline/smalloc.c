#include <stdlib.h>
#include <stdio.h>

#include "smalloc.h"

void* smalloc(int size){

	void* res = malloc(size);
	if(res == NULL){
		printf("could not malloc.");
		exit(1);
	}
	return res;
}