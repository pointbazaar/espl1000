#include <stdio.h>

#include "code_gen_util.h"

void indent(FILE* file, int indentLevel){
	
	for(int i=0;i < indentLevel; i++){
		fprintf(file, "\t");
	}
}
