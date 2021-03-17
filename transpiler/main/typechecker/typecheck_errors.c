#include <stdio.h>
#include <stdlib.h>

#include "typecheck_errors.h"

void error(char* msg){

	printf("[Typecheck][Error] %s\n", msg);
	exit(1);
}
