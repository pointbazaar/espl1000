#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){

	char call[1000];
	
	call[0]='\0';

	strcat(call,"java -jar ~/dragon/target/dragon-0.1-jar-with-dependencies.jar ");

	for(int i=0;i<argc;i++){
		strcat(call,argv[i]);
		strcat(call," ");
	}	

	system(call);

	return 0;
}
