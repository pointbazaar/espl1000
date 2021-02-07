#include "driver.h"

#include <stdio.h>

int driver(){
	
	printf("running driver program\n");
	
	return 0;
}

void out(int id, char* str){
	fprintf(outFile, "%d %s\n", id, str);
}

void out2(int id, int id2){
	fprintf(outFile, "%d %d\n", id, id2);
}
