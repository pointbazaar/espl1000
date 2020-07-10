#include <stdlib.h>
#include <stdio.h>

#include "java_code_gen.h"

int gen_java_code(struct AST_Whole_Program* ast, char* fname_out){

	printf("writing to: %s\n", fname_out);
	//TODO
	FILE* fout = fopen(fname_out, "w");

	const int ns_count = ast->count_namespaces;

	//for each namespace, we need a new java file
	for(int i=0;i < ns_count; i++){
			gen_java_namespace(ast->namespaces[i], fout);
	}

	fclose(fout);
	return 0;
}


void gen_java_namespace(struct Namespace* ns, FILE* file){
	
	fprintf(file, "public class %s {\n", ns->name);
	
	fprintf(file, "}\n");
}
