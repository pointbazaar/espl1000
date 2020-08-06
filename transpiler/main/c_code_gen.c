#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c_code_gen.h"
#include "code_gen_util.h"

//methods delcared in .c file, as they should be private
//to this file.

void transpileAST(struct AST_Whole_Program* ast, FILE* file);
void transpileNamespace(struct Namespace* ns, FILE* file);

void transpileStructDecl(struct StructDecl* s, FILE* file);
void transpileStructMember(struct StructMember* m, FILE* file);

void transpileMethod(struct Method* m, FILE* file);
void transpileMethodSignature(struct Method* m, FILE* file);

void transpileStmtBlock(struct StmtBlock* block, FILE* file, int indent);
void transpileStmt(struct Stmt* s, FILE* file, int indentLevel);

//stmt related
void transpileMethodCall(struct MethodCall* mc, FILE* file, int indentLevel);
void transpileWhileStmt(struct WhileStmt* ws, FILE* file, int indentLevel);
void transpileIfStmt(struct IfStmt* is, FILE* file, int indentLevel);
void transpileRetStmt(struct RetStmt* rs, FILE* file, int indentLevel);
void transpileAssignStmt(struct AssignStmt* as, FILE* file, int indentLevel);

void transpileType(struct Type* t, FILE* file);
void transpileVariable(struct Variable* var, FILE* file);
void transpileTerm(struct Term* expr, FILE* file);
void transpileExpr(struct Expr* expr, FILE* file);

void transpileSimpleVar(struct SimpleVar* svar, FILE* file);

void transpileBoolConst		(struct BoolConst* bc, 	FILE* file);
void transpileIntConst		(struct IntConst* ic, 	FILE* file);
void transpileCharConst		(struct CharConst* cc, 	FILE* file);
void transpileFloatConst	(struct FloatConst* fc, FILE* file);
void transpileStringConst	(struct StringConst* s, FILE* file);

void transpileOp(struct Op* op, FILE* file);

void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, FILE* file);
void transpileTypeParam(struct TypeParam* tp, FILE* file);
void transpileArrayType(struct ArrayType* atype, FILE* file);

void transpileSimpleType(struct SimpleType* simpleType, FILE* file);
void transpileSubrType(struct SubrType* subrType, FILE* file);

void transpileDeclArg(struct DeclArg* da, FILE* file);

void transpileAndWrite(char* filename, struct AST_Whole_Program* ast){
	
	//TODO: use debug param
	printf("transpileAndWrite(...)\n");

	FILE* fout = fopen(filename, "w");

	if(fout == NULL){
		printf("could not open output file\n");
		exit(1);
	}

	transpileAST(ast, fout);


	fclose(fout);
}

void transpileAST(struct AST_Whole_Program* ast, FILE* file){
	
	printf("transpileAST(...)\n");
	
	//write some standard stdlib includes
	fprintf(file, "#include <stdlib.h>\n");
	fprintf(file, "#include <stdio.h>\n");
	fprintf(file, "#include <stdbool.h>\n");
	fprintf(file, "#include <string.h>\n");

	for(int i=0; i < ast->count_namespaces; i++){

		transpileNamespace(ast->namespaces[i], file);
	}
}

void transpileNamespace(struct Namespace* ns, FILE* file){
	
	printf("transpileNamespace(...)\n");
	
	//write struct forward declarations
	for(int i=0;i < ns->count_structs; i++){
		fprintf(file, "struct %s;\n", ns->structs[i]->name);
	}

	//transpile struct definitions
	for(int i=0;i < ns->count_structs; i++){
		transpileStructDecl(ns->structs[i], file);
	}
	
	//write subroutine forward declarations
	for(int i=0; i < ns->count_methods; i++){
		transpileMethodSignature(ns->methods[i], file);
		fprintf(file, ";\n");
	}

	for(int i=0; i < ns->count_methods; i++){
		transpileMethod(ns->methods[i], file);
	}
}

void transpileStructDecl(struct StructDecl* s, FILE* file){
	
	printf("transpileStructDecl(...)\n");
	fprintf(file ,"struct %s {\n", s->name);
	
	for(int i=0;i < s->count_members;i++){
		transpileStructMember(s->members[i], file);
	}
	
	fprintf(file, "}\n");
}

void transpileStructMember(struct StructMember* m, FILE* file){
	
	printf("transpileStructMember(...)\n");
	
	fprintf(file, "\t");
	
	transpileType(m->type, file);
	
	fprintf(file, " %s;\n", m->name);
}

void transpileMethod(struct Method* m, FILE* file){
	
	printf("transpileMethod(...)\n");

	transpileMethodSignature(m, file);

	transpileStmtBlock(m->block, file, 0);
}

void transpileMethodSignature(struct Method* m, FILE* file){
	
	printf("transpileMethodSignature(...)\n");
	
	transpileType(m->returnType, file);

	fprintf(file, " %s(", m->name);

	for(int i=0; i < m->count_args; i++){
		transpileDeclArg(m->args[i], file);
		if(i < (m->count_args)-1){
			fprintf(file, ", ");
		}
	}

	fprintf(file, ")");
}

void transpileStmtBlock(struct StmtBlock* block, FILE* file, int indentLevel){
	
	printf("transpileStmtBlock(...)\n");

	fprintf(file, "{\n");

	for(int i=0; i < block->count; i++){
		transpileStmt(block->stmts[i], file, indentLevel+1);
	}

	indent(file, indentLevel);
	fprintf(file, "}\n");
}

void transpileStmt(struct Stmt* s, FILE* file, int indentLevel){
	
	printf("transpileStmt(...)\n");

	if(s->m1 != NULL){
		transpileMethodCall(s->m1, file, indentLevel);
		fprintf(file, ";");
	}else if(s->m2 != NULL){
		transpileWhileStmt(s->m2, file, indentLevel);
	}else if(s->m3 != NULL){
		transpileIfStmt(s->m3, file, indentLevel);
	}else if(s->m4 != NULL){
		transpileRetStmt(s->m4, file, indentLevel);
		fprintf(file, ";");
	}else if(s->m5 != NULL){
		transpileAssignStmt(s->m5, file, indentLevel);
		fprintf(file, ";");
	}else{
		printf("Error in transpileStmt\n");
		exit(1);
	}
	fprintf(file, "\n");
}

//stmt related

void transpileMethodCall(struct MethodCall* mc, FILE* file, int indentLevel){
	
	printf("transpileMethodCall(...)\n");
	
	indent(file, indentLevel);
	fprintf(file, "%s(", mc->methodName);

	for(int i=0;i < mc->count_args; i++){
		transpileExpr(mc->args[i], file);
		if(i < (mc->count_args)-1){
			fprintf(file, ", ");
		}
	}

	fprintf(file, ")");
}

void transpileWhileStmt(struct WhileStmt* ws, FILE* file, int indentLevel){
	
	printf("transpileWhileStmt(...)\n");
	
	indent(file, indentLevel);
	
	fprintf(file, "while (");
	transpileExpr(ws->condition, file);
	fprintf(file, ")");

	transpileStmtBlock(ws->block, file, indentLevel);
}

void transpileIfStmt(struct IfStmt* is, FILE* file, int indentLevel){
	
	printf("transpileIfStmt(...)\n");
	
	indent(file, indentLevel);
	
	fprintf(file, "if (");
	transpileExpr(is->condition, file);
	fprintf(file, ")");
		transpileStmtBlock(is->block, file, indentLevel);
	
	if(is->elseBlock != NULL){
		
		indent(file, indentLevel);
		fprintf(file, "else");
			transpileStmtBlock(is->elseBlock, file, indentLevel);
	}
}

void transpileRetStmt(struct RetStmt* rs, FILE* file, int indentLevel){
	
	printf("transpileRetStmt(...)\n");
	
	indent(file, indentLevel);
	
	fprintf(file, "return ");
	transpileExpr(rs->returnValue, file);
}

void transpileAssignStmt(struct AssignStmt* as, FILE* file, int indentLevel){
	
	printf("transpileAssignStmt(...)\n");

	indent(file, indentLevel);

	if(as->optType != NULL){
		transpileType(as->optType, file);
		
		fprintf(file, " ");
	}
	
	transpileVariable(as->var, file);
	fprintf(file, " = ");
	transpileExpr(as->expr, file);
}

void transpileType(struct Type* t, FILE* file){
	
	printf("transpileType(...)\n");
	
	if(t->m1 != NULL){
		transpileBasicTypeWrapped(t->m1, file);
	}else if(t->m2 != NULL){
		transpileTypeParam(t->m2, file);
	}else if(t->m3 != NULL){
		transpileArrayType(t->m3, file);
	}
}

void transpileVariable(struct Variable* var, FILE* file){
	
	printf("transpileVariable(...)\n");
	
	transpileSimpleVar(var->simpleVar, file);
	for(int i=0; i < var->count_memberAccessList; i++){
		fprintf(file, ".");
		transpileVariable(var->memberAccessList[i], file);
	}
}

void transpileTerm(struct Term* t, FILE* file){
	
	printf("transpileTerm(...)\n");
	
	if(t->m1 != NULL){
		transpileBoolConst(t->m1, file);
	}else if(t->m2 != NULL){
		transpileIntConst(t->m2, file);
	}else if(t->m3 != NULL){
		transpileCharConst(t->m3, file);
	}else if(t->m4 != NULL){
		transpileMethodCall(t->m4, file, 0);
	}else if(t->m5 != NULL){
		transpileExpr(t->m5, file);
	}else if(t->m6 != NULL){
		transpileVariable(t->m6, file);
	}else if(t->m7 != NULL){
		transpileFloatConst(t->m7, file);
	}else if(t->m8 != NULL){
		transpileStringConst(t->m8, file);
	}else{
		printf("Error in transpileTerm\n");
		exit(1);
	}
}

void transpileExpr(struct Expr* expr, FILE* file){
	
	printf("transpileExpr(...)\n");

	transpileTerm(expr->term1, file);

	if(expr->op != NULL){
		transpileOp(expr->op, file);
		transpileTerm(expr->term2, file);
	}
}

void transpileSimpleVar(struct SimpleVar* svar, FILE* file){
	
	printf("transpileSimpleVar(...)\n");
	
	fprintf(file, "%s", svar->name);
	if(svar->optIndex != NULL){
		fprintf(file, "[");
		transpileExpr(svar->optIndex, file);
		fprintf(file, "]");
	}
}

void transpileBoolConst(struct BoolConst* bc, FILE* file){
	
	printf("transpileBoolConst(...)\n");
	
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
	fprintf(file, "'%c'", cc->value);
}

void transpileFloatConst(struct FloatConst* fc, FILE* file){
	fprintf(file, "%f", fc->value);
}

void transpileStringConst	(struct StringConst* s, FILE* file){
	//fprintf(file, "\"%s\"", s->value);
	
	//quotation seems to already be present
	fprintf(file, "%s", s->value);
}

void transpileOp(struct Op* op, FILE* file){
	fprintf(file, " %s ", op->op);
}

void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, FILE* file){
	
	printf("transpileBasicTypeWrapped(...)\n");
	
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
	
	printf("transpileArrayType(...)\n");
	
	transpileType(atype->element_type, file);
	fprintf(file, "*");
}

void transpileSimpleType(struct SimpleType* simpleType, FILE* file){
	
	printf("transpileSimpleType(...)\n");
	
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
	}else if(
		strcmp(tname, "Bool") == 0
	){
		strcpy(res, "bool");
	}else{
		strcpy(res, tname);
	}

	fprintf(file, "%s", res);
}

void transpileSubrType(struct SubrType* subrType, FILE* file){
	
	//https://www.zentut.com/c-tutorial/c-function-pointer/
	
	printf("transpileSubrType(...)\n");

	//return type
	transpileType(subrType->returnType, file);

	//TODO: i do not really understand how
	//this is written in C. 
	//maybe trying some examples in C would help.
	fprintf(file, "(*function_ptr) (");

	//arguments
	for(int i=0; i < subrType->count_argTypes; i++){
		transpileType(subrType->argTypes[i], file);
		
		if(i < (subrType->count_argTypes)-1){
			fprintf(file, ", ");
		}
	}
	fprintf(file, ")");
}

void transpileDeclArg(struct DeclArg* da, FILE* file){
	
	printf("transpileDeclArg(...)\n");
	
	transpileType(da->type, file);

	if(da->has_name){
		fprintf(file, " %s", da->name);
	}
}
