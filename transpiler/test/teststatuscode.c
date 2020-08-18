#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "teststatuscode.h"
#include "../main/flags.h"
#include "../main/transpiler.h"

void clean();

int sourceToStatus(char* src, bool debug){
	
	if(debug){ printf("sourceToStatus(...)\n"); }
	
	clean();
	
	//write this to a file
	FILE* file = fopen("test.dg","w");
	
	if(file == NULL){
		printf("could not open output file\n");
		exit(1);
	}
	
	fprintf(file, "%s", src);
	fclose(file);
	
	//transpile it
	struct Flags* flags = malloc(sizeof(struct Flags));
	flags->debug = false;
	flags->avr = false;
	flags->test = false;
	char** gcc_flags = NULL;
	int gcc_flags_count = 0;
	
	transpileAndCompile("test.dg", flags, gcc_flags, gcc_flags_count);
	
	//test that the code, when transpiled,
	//returns a status code of 3
	int status = WEXITSTATUS(system("./a.out"));
	
	clean();
	
	free(flags);
	
	return status;
}

void clean(){
	
	system("rm test.dg");
	system("rm test.c");
	system("rm a.out");
}
