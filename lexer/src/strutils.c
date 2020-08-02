#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "strutils.h"

char* substr(char* str, int len){
	//returns the substring, up to the length specified
	char* res = malloc((len+1)* sizeof(char) );
	
	memcpy(res,str,len*sizeof(char));
	
	res[len]='\0';
	
	return res;
	
}
