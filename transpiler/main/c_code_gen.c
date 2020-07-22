#include <stdio.h>
#include <stdlib.h>

#include "c_code_gen.h"

void transpileAST(struct AST_Whole_Program* ast, FILE* file);
void transpileNamespace(struct Namespace* ns, FILE* file);
void transpileMethod(struct Method* m, FILE* file);
void transpileStmtBlock(struct StmtBlock* block, FILE* file);
void transpileStmt(struct Stmt* s, FILE* file);

void transpileAndWrite(char* filename, struct AST_Whole_Program* ast){

	FILE* fout = fopen(filename, "w");

	if(fout == NULL){
		printf("could not open output file\n");
		exit(1);
	}

	transpileAST(ast, fout);


	fclose(fout);
}

void transpileAST(struct AST_Whole_Program* ast, FILE* file){
	
	//write some standard stdlib includes
	fprintf(file, "#include <stdlib.h>\n");
	fprintf(file, "#include <stdio.h>\n");
	fprintf(file, "#include <stdbool.h>\n");

	//TODO: write struct declarations

	//TODO: write struct definitions

	//TODO: write subroutine declarations

	for(int i=0; i < ast->count_namespaces; i++){

		transpileNamespace(ast->namespaces[i], file);
	}
}

void transpileNamespace(struct Namespace* ns, FILE* file){

	//TODO: transpile struct definitions

	for(int i=0; i < ns->count_methods; i++){

		transpileMethod(ns->methods[i], file);
	}
}

void transpileMethod(struct Method* m, FILE* file){

	fprintf(file, "void %s(", m->name);

	for(int i=0; i < m->count_args; i++){
		//TODO
	}

	fprintf(file, ")");

	transpileStmtBlock(m->block, file);
}

void transpileStmtBlock(struct StmtBlock* block, FILE* file){

	fprintf(file, "{\n");

	for(int i=0; i < block->count; i++){
		transpileStmt(block->stmts[i], file);
	}

	fprintf(file, "}\n");
}

void transpileStmt(struct Stmt* s, FILE* file){

	//TODO
}