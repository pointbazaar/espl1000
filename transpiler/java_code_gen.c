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
	
	printf("gen_java_namespace(...)\n");
	
	fprintf(file, "public class %s {\n", ns->name);
	
	//for each subroutine, generate a static method in java
	
	fprintf(file, "}\n");
}

void gen_java_method(struct Method* m, FILE* file){
	
	printf("gen_java_method(...)\n");
	
	fprintf(file, "public static %s %s (", "int", m->methodName);
	
	
	for(int i=0;i < m->count_args; i++){
			//TODO: print argument
	}
	
	fprintf(file, "){\n");
	
	for(int i=0;i < m->count_stmts; i++){
			//TODO: print statement
	}
	
	fprintf(file, "}\n");
}
