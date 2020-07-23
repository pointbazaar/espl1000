#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c_code_gen.h"

//methods delcared in .c file, as they should be private
//to this file.

void transpileAST(struct AST_Whole_Program* ast, FILE* file);
void transpileNamespace(struct Namespace* ns, FILE* file);
void transpileMethod(struct Method* m, FILE* file);
void transpileStmtBlock(struct StmtBlock* block, FILE* file);
void transpileStmt(struct Stmt* s, FILE* file);

//stmt related
void transpileMethodCall(struct MethodCall* mc, FILE* file);
void transpileWhileStmt(struct WhileStmt* ws, FILE* file);
void transpileIfStmt(struct IfStmt* is, FILE* file);
void transpileRetStmt(struct RetStmt* rs, FILE* file);
void transpileAssignStmt(struct AssignStmt* as, FILE* file);

void transpileType(struct Type* t, FILE* file);
void transpileVariable(struct Variable* var, FILE* file);
void transpileTerm(struct Term* expr, FILE* file);
void transpileExpr(struct Expr* expr, FILE* file);

void transpileSimpleVar(struct SimpleVar* svar, FILE* file);

void transpileBoolConst(struct BoolConst* bc, FILE* file);
void transpileIntConst(struct IntConst* ic, FILE* file);
void transpileCharConst(struct CharConst* cc, FILE* file);
void transpileFloatConst(struct FloatConst* fc, FILE* file);

void transpileOp(struct Op* op, FILE* file);

void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, FILE* file);
void transpileTypeParam(struct TypeParam* tp, FILE* file);
void transpileArrayType(struct ArrayType* atype, FILE* file);

void transpileSimpleType(struct SimpleType* simpleType, FILE* file);
void transpileSubrType(struct SubrType* subrType, FILE* file);

void transpileDeclArg(struct DeclArg* da, FILE* file);

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
		transpileDeclArg(m->args[i], file);
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

	if(s->m1 != NULL){
		transpileMethodCall(s->m1, file);
	}else if(s->m2 != NULL){
		transpileWhileStmt(s->m2, file);
	}else if(s->m3 != NULL){
		transpileIfStmt(s->m3, file);
	}else if(s->m4 != NULL){
		transpileRetStmt(s->m4, file);
	}else if(s->m5 != NULL){
		transpileAssignStmt(s->m5, file);
	}
	fprintf(file, ";\n");
}

//stmt related

void transpileMethodCall(struct MethodCall* mc, FILE* file){
	
	fprintf(file, "%s(", mc->methodName);

	for(int i=0;i < mc->count_args; i++){
		transpileExpr(mc->args[i], file);
	}

	fprintf(file, ")");
}

void transpileWhileStmt(struct WhileStmt* ws, FILE* file){
	
	fprintf(file, "while (");
	transpileExpr(ws->condition, file);
	fprintf(file, ")");

	transpileStmtBlock(ws->block, file);
}

void transpileIfStmt(struct IfStmt* is, FILE* file){
	
	fprintf(file, "if (");
	transpileExpr(is->condition, file);
	fprintf(file, ")");
		transpileStmtBlock(is->block, file);
	fprintf(file, "else");
		transpileStmtBlock(is->elseBlock, file);
}

void transpileRetStmt(struct RetStmt* rs, FILE* file){
	
	fprintf(file, "return ");
	transpileExpr(rs->returnValue, file);
}

void transpileAssignStmt(struct AssignStmt* as, FILE* file){

	if(as->optType != NULL){

		transpileType(as->optType, file);
	}
	transpileVariable(as->var, file);
	fprintf(file, " = ");
	transpileExpr(as->expr, file);
}

void transpileType(struct Type* t, FILE* file){
	
	if(t->m1 != NULL){
		transpileBasicTypeWrapped(t->m1, file);
	}else if(t->m2 != NULL){
		transpileTypeParam(t->m2, file);
	}else if(t->m3 != NULL){
		transpileArrayType(t->m3, file);
	}
}

void transpileVariable(struct Variable* var, FILE* file){
	
	transpileSimpleVar(var->simpleVar, file);
	for(int i=0; i < var->count_memberAccessList; i++){
		fprintf(file, ".");
		transpileVariable(var->memberAccessList[i], file);
	}
}

void transpileTerm(struct Term* t, FILE* file){
	
	if(t->m1 != NULL){
		transpileBoolConst(t->m1, file);
	}else if(t->m2 != NULL){
		transpileIntConst(t->m2, file);
	}else if(t->m3 != NULL){
		transpileCharConst(t->m3, file);
	}else if(t->m4 != NULL){
		transpileMethodCall(t->m4, file);
	}else if(t->m5 != NULL){
		transpileExpr(t->m5, file);
	}else if(t->m6 != NULL){
		transpileVariable(t->m6, file);
	}else if(t->m7 != NULL){
		transpileFloatConst(t->m7, file);
	}
}

void transpileExpr(struct Expr* expr, FILE* file){

	transpileTerm(expr->term1, file);

	if(expr->op != NULL){
		transpileOp(expr->op, file);
		transpileTerm(expr->term2, file);
	}
}

void transpileSimpleVar(struct SimpleVar* svar, FILE* file){
	
	fprintf(file, "%s", svar->name);
	if(svar->optIndex != NULL){
		fprintf(file, "[");
		transpileExpr(svar->optIndex, file);
		fprintf(file, "]");
	}
}

void transpileBoolConst(struct BoolConst* bc, FILE* file){
	
	if(bc->value){
		fprintf(file, "true");
	}else{
		fprintf(file, "false");
	}
}

void transpileIntConst(struct IntConst* ic, FILE* file){
	fprintf(file, "%d", ic->value);
}

void transpileCharConst(struct CharConst* cc, FILE* file){
	fprintf(file, "%c", cc->value);
}

void transpileFloatConst(struct FloatConst* fc, FILE* file){
	fprintf(file, "%f", fc->value);
}

void transpileOp(struct Op* op, FILE* file){
	fprintf(file, "%s", op->op);
}

void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, FILE* file){
	
	if(btw->simpleType != NULL){
		transpileSimpleType(btw->simpleType, file);
	}else if(btw->subrType != NULL){
		transpileSubrType(btw->subrType, file);
	}
}

void transpileTypeParam(struct TypeParam* tp, FILE* file){
	//TODO
	printf("transpileTypeParam not yet implemented!\n");
	exit(1);
}

void transpileArrayType(struct ArrayType* atype, FILE* file){
	
	transpileType(atype->element_type, file);
	fprintf(file, "*");
}

void transpileSimpleType(struct SimpleType* simpleType, FILE* file){
	
	char* tname = simpleType->typeName;
	char res[20];

	if(
		strcmp(tname, "PInt") == 0
		|| strcmp(tname, "NInt") == 0
		|| strcmp(tname, "Int") == 0
		|| strcmp(tname, "NZInt") == 0
	){
		strcpy(res, "int");
	}else if(
		strcmp(tname, "PFloat") == 0
		|| strcmp(tname, "NFloat") == 0
		|| strcmp(tname, "Float") == 0
	){
		strcpy(res, "float");
	}else{
		strcpy(res, tname);
	}

	fprintf(file, "%s", res);
}

void transpileSubrType(struct SubrType* subrType, FILE* file){

	//return type
	transpileType(subrType->returnType, file);

	//TODO: i do not really understand how
	//this is written in C. 
	//maybe trying some examples in C would help.
	fprintf(file, "(*function_ptr(");

	//arguments
	for(int i=0; i < subrType->count_argTypes; i++){
		transpileType(subrType->argTypes[i], file);
	}
	fprintf(file, "))");
}

void transpileDeclArg(struct DeclArg* da, FILE* file){
	
	transpileType(da->type, file);

	if(da->has_name){
		fprintf(file, " %s", da->name);
	}
}