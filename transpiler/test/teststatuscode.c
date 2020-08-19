#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../util/util.h"

#include "teststatuscode.h"
#include "../main/flags.h"
#include "../main/transpiler.h"

void clean();

char* FNAME_DEFAULT = "test.dg";

int sourceToStatus(char* src, bool debug){
	
	if(debug){ printf("sourceToStatus(...)\n"); }
	
	clean();
	
	//write this to a file
	FILE* file = fopen(FNAME_DEFAULT,"w");
	
	if(file == NULL){
		printf("could not open output file\n");
		exit(1);
	}
	
	fprintf(file, "%s", src);
	fclose(file);
	
	//transpile it
	struct Flags* flags = smalloc(sizeof(struct Flags));
	flags->debug = false;
	flags->avr = false;
	flags->test = false;
	char** gcc_flags = NULL;
	int gcc_flags_count = 0;
	
	transpileAndCompile(FNAME_DEFAULT, flags, gcc_flags, gcc_flags_count);
	
	//test that the code, when transpiled,
	//returns a status code of 3
	int status = WEXITSTATUS(system("./a.out"));
	
	if(!debug){
		//in debug mode, leave the artifacts
		//so we can look at them
		clean();
	}
	
	free(flags);
	
	return status;
}

void clean(){
	
	// '-f ' option so that it will not give a warning
	
	system("rm -f test.dg");
	system("rm -f test.c");
	system("rm -f a.out");
}
