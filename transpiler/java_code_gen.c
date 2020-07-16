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
	
	//TODO: handle the structures
	
	//for each subroutine, generate a static method in java
	for(int i=0;i<ns->count_methods;i++){
		gen_java_method(ns->methods[i], file);
	}
	
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
			gen_java_stmt(m->stmts[i], file);
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

void gen_java_stmt(struct Stmt* stmt, FILE* file){

	if(stmt->m1 != NULL){
		gen_java_methodcall(stmt->m1, file);
	}else if(stmt->m2 != NULL){
		gen_java_whilestmt(stmt->m2, file);
	}else if(stmt->m3 != NULL){
		gen_java_ifstmt(stmt->m3, file);
	}else if(stmt->m4 != NULL){
		gen_java_retstmt(stmt->m4, file);
	}else if(stmt->m5 != NULL){
		gen_java_assignstmt(stmt->m5, file);
	}
	
	//newline, ';' at the end
	fprintf(file, ";\n");
}

void gen_java_methodcall(struct MethodCall* m, FILE* file){
	fprintf(file, "%s(", m->methodName);
	
	for(int i=0;i<m->count_args;i++){
		gen_java_expr(m->args[i], file);
	}
	fprintf(file, ")");
}
void gen_java_whilestmt(struct WhileStmt* m, FILE* file){
	fprintf(file, "while (");
	gen_java_expr(m->condition, file);
	fprintf(file, ") {\n");
	
	for(int i=0;i<m->count_statements;i++){
		gen_java_stmt(m->statements[i], file);
	}
	
	fprintf(file, "}");
}
void gen_java_ifstmt(struct IfStmt* m, FILE* file){
	fprintf(file, "if (");
	gen_java_expr(m->condition, file);
	fprintf(file, ") {\n");
	
	for(int i=0;i<m->count_statements;i++){
		gen_java_stmt(m->statements[i], file);
	}
	
	fprintf(file, "}");
	
	fprintf(file, "else if {\n");
	
	for(int i=0;i<m->count_elseStatements;i++){
		gen_java_stmt(m->elseStatements[i], file);
	}
	
	fprintf(file, "}");
}
void gen_java_retstmt(struct RetStmt* m, FILE* file){
	fprintf(file, "return ");
	gen_java_expr(m->returnValue, file);
}
void gen_java_assignstmt(struct AssignStmt* m, FILE* file){
	if(m->optType != NULL){
		gen_java_type(m->optType, file);
	}
	gen_java_var(m->var, file);
	fprintf(file, " = ");
	
	gen_java_expr(m->expr, file);
}


void gen_java_var(struct Variable* m, FILE* file){
	//TODO
	printf("not implemented yet! (java_code_gen.c)\n");
	exit(1);
}
void gen_java_expr(struct Expr* m, FILE* file){
	//TODO
	printf("not implemented yet! (java_code_gen.c)\n");
	exit(1);
}
