#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

#include "../../util/util.h"

#include "teststatuscode.h"
#include "../main/flags.h"
#include "../main/transpiler.h"

void clean();

char* FNAME_DEFAULT = "test.dg";

int sourceToStatus(char* src, bool debug){
	
	if(debug){ printf("sourceToStatus(...)\n"); }
	
	clean();
	
	
	FILE* file = fopen(FNAME_DEFAULT,"w");
	
	if(file == NULL){
		printf("could not open output file %s\n", FNAME_DEFAULT);
		
		return -1;
	}
	
	fprintf(file, "%s", src);
	fclose(file);
	
	//transpile it
	struct Flags* flags = makeFlags(0, NULL);
	flags->debug = debug;
	
	transpileAndCompile(FNAME_DEFAULT, flags);
	
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
	system("rm -f .test.dg.tokens");
	system("rm -f .test.dg.ast");
	system("rm -f test.c");
	system("rm -f a.out");
}
