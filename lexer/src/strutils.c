//stdlib includes
#include <stdlib.h> //for malloc
#include <string.h> //for memcpy

//project includes
#include "strutils.h"

char* substr(char* str, int len){
	//returns the substring, up to the length specified
	char* res = malloc((len+1)* sizeof(char) );
	
	memcpy(res,str,len*sizeof(char));
	
	res[len]='\0';
	
	return res;
	
}
