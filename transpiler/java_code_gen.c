#include <stdlib.h>
#include <stdio.h>

#include "java_code_gen.h"

int gen_java_code(struct AST_Whole_Program* ast, char* fname_out){

	printf("writing to: %s\n", fname_out);
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

void gen_java_type(struct Type* t, FILE* file){
	
	if(t->m1 != NULL){
		gen_java_basictypewrapped(t->m1, file);
	}else if(t->m2 != NULL){
		gen_java_typeparam(t->m2, file);
	}else if(t->m3 != NULL){
		gen_java_arraytype(t->m3, file);
	}
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
	gen_java_simplevar(m->simpleVar, file);
	for(int i=0;i < m->count_memberAccessList;i++){
		fprintf(file, ".");
		gen_java_var(m->memberAccessList[i], file);
	}
}
void gen_java_expr(struct Expr* m, FILE* file){
	gen_java_term(m->term1, file);
	if(m->op != NULL){
		gen_java_op(m->op, file);
		gen_java_term(m->term2, file);
	}
}
void gen_java_simplevar(struct SimpleVar* m, FILE* file){
	fprintf(file, "%s", m->name);
	if(m->optIndex != NULL){
		fprintf(file, "[");
		gen_java_expr(m->optIndex, file);
		fprintf(file, "]");
	}
}

void gen_java_op(struct Op* m, FILE* file){
	fprintf(file, " %s ", m->op);
}
void gen_java_term(struct Term* t, FILE* file){
	if(t->m1 != NULL){
		gen_java_boolconst(t->m1, file);
	}else if(t->m2 != NULL){
		gen_java_intconst(t->m2, file);
	}else if(t->m3 != NULL){
		gen_java_charconst(t->m3, file);
	}else if(t->m4 != NULL){
		gen_java_methodcall(t->m4, file);
	}else if(t->m5 != NULL){
		gen_java_expr(t->m5, file);
	}else if(t->m6 != NULL){
		gen_java_var(t->m6, file);
	}else if(t->m7 != NULL){
		gen_java_floatconst(t->m7, file);
	}
}
// constant -------

void gen_java_boolconst(struct BoolConst* m, FILE* file){
	fprintf(file, "%d", m->value);
}
void gen_java_intconst(struct IntConst* m, FILE* file){
	fprintf(file, "%d", m->value);
}
void gen_java_charconst(struct CharConst* m, FILE* file){
	fprintf(file, "%c", m->value);
}
void gen_java_floatconst(struct FloatConst* m, FILE* file){
	fprintf(file, "%f", m->value);
}

// other type stuff 

void gen_java_basictypewrapped(struct BasicTypeWrapped* t, FILE* file){
	if(t->simpleType != NULL){
		gen_java_simpleType(t->simpleType, file);
	}else if(t->subrType != NULL){
		gen_java_subrType(t->subrType, file);
	}
}
void gen_java_typeparam(struct TypeParam* m, FILE* file){
	fprintf(file, "T%d", m->index);
}
void gen_java_arraytype(struct ArrayType* m, FILE* file){
	gen_java_type(m->element_type, file);
	fprintf(file, "[]");
}

void gen_java_simpleType(struct SimpleType* m, FILE* file){
	fprintf(file, "%s", m->typeName);
}
void gen_java_subrType(struct SubrType* m, FILE* file){
	//TODO
	printf("failure in java_code_gen.c : could not print subroutine.\n");
	exit(1);
}
