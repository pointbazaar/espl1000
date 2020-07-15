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
	
	fprintf(file, "public static %s %s (", "int", m->name);
	
	
	for(int i=0;i < m->count_args; i++){
		
			struct DeclArg* da = m->args[i];
			gen_java_declarg(da, file);
			if(i < (m->count_args - 1)){
				fprintf(file, ", ");
			}
	}
	
	fprintf(file, "){\n");
	
	for(int i=0;i < m->count_stmts; i++){
			//TODO: print statement
			printf("not implemented yet! (java_code_gen.c)\n");
			exit(1);
	}
	
	fprintf(file, "}\n");
}

// --------------------------------

void gen_java_declarg(struct DeclArg* da, FILE* file){
	
	printf("gen_java_declarg(...)\n");
	
	gen_java_type(da->type, file);
	if(da->has_name){
		fprintf(file, " %s", da->name);
	}
}

void gen_java_type(struct Type* type, FILE* file){
	//TODO
	printf("not implemented yet! (java_code_gen.c)\n");
	exit(1);
}
