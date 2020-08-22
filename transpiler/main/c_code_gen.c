#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c_code_gen.h"
#include "code_gen_util.h"
#include "../../util/util.h"

//methods delcared in .c file, as they should be private
//to this file.

void transpileAST(struct AST_Whole_Program* ast, struct Ctx* ctx);
void transpileNamespace(struct Namespace* ns, struct Ctx* ctx);

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx);
void transpileStructMember(struct StructMember* m, struct Ctx* ctx);

void transpileMethod(struct Method* m, struct Ctx* ctx);
void transpileMethodSignature(struct Method* m, struct Ctx* ctx);

void transpileStmtBlock(struct StmtBlock* block, struct Ctx* ctx);
void transpileStmt(struct Stmt* s, struct Ctx* ctx);

//stmt related
void transpileMethodCall(struct MethodCall* mc, struct Ctx* ctx);
void transpileWhileStmt(struct WhileStmt* ws, struct Ctx* ctx);
void transpileIfStmt(struct IfStmt* is, struct Ctx* ctx);
void transpileRetStmt(struct RetStmt* rs, struct Ctx* ctx);
void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx);

void transpileType(struct Type* t, struct Ctx* ctx);
void transpileVariable(struct Variable* var, struct Ctx* ctx);
void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx);
void transpileTerm(struct Term* expr, struct Ctx* ctx);
void transpileExpr(struct Expr* expr, struct Ctx* ctx);

void transpileSimpleVar(struct SimpleVar* svar, struct Ctx* ctx);

void transpileBoolConst		(struct BoolConst* bc, 	struct Ctx* ctx);
void transpileIntConst		(struct IntConst* ic, 	struct Ctx* ctx);
void transpileCharConst		(struct CharConst* cc, 	struct Ctx* ctx);
void transpileFloatConst	(struct FloatConst* fc, struct Ctx* ctx);
void transpileStringConst	(struct StringConst* s, struct Ctx* ctx);

void transpileOp(struct Op* op, struct Ctx* ctx);

void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, struct Ctx* ctx);
void transpileTypeParam(struct TypeParam* tp, struct Ctx* ctx);
void transpileArrayType(struct ArrayType* atype, struct Ctx* ctx);

void transpileSimpleType(struct SimpleType* simpleType, struct Ctx* ctx);
void transpileSubrType(struct SubrType* subrType, struct Ctx* ctx);

void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx);

void transpileAndWrite(char* filename, struct AST_Whole_Program* ast, struct Flags* flags){
	
	bool debug = flags->debug;
	
	if(debug){ printf("transpileAndWrite(...)\n"); }

	FILE* fout = fopen(filename, "w");

	if(fout == NULL){
		printf("could not open output file: %s\n", filename);
		exit(1);
	}
	
	struct Ctx* ctx = smalloc(sizeof(struct Ctx));
	ctx->flags = flags;
	ctx->file = fout;
	ctx->indentLevel = 0;

	transpileAST(ast, ctx);

	fclose(fout);
	
	if(debug){ printf("transpileAndWrite(...) DONE\n"); }
}

void transpileAST(struct AST_Whole_Program* ast, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileAST(...)\n"); }
	
	//write some standard stdlib includes
	FILE* file = ctx->file;
	
	if(!(ctx->flags->avr)){
		//in microcontrollers, we cannot assume there will
		//be stdlib 
		
		fprintf(file, "#include <stdlib.h>\n");
		fprintf(file, "#include <stdio.h>\n");
		fprintf(file, "#include <stdbool.h>\n");
		fprintf(file, "#include <string.h>\n");
		fprintf(file, "#include <math.h>\n");
	}

	for(int i=0; i < ast->count_namespaces; i++){

		transpileNamespace(ast->namespaces[i], ctx);
	}
}

void transpileNamespace(struct Namespace* ns, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileNamespace(...)\n"); }
	
	FILE* file = ctx->file;
	
	//write struct forward declarations
	for(int i=0;i < ns->count_structs; i++){
		fprintf(file, "struct %s;\n", ns->structs[i]->name);
	}

	//transpile struct definitions
	for(int i=0;i < ns->count_structs; i++){
		transpileStructDecl(ns->structs[i], ctx);
	}
	
	//write subroutine forward declarations
	for(int i=0; i < ns->count_methods; i++){
		transpileMethodSignature(ns->methods[i], ctx);
		fprintf(file, ";\n");
	}

	for(int i=0; i < ns->count_methods; i++){
		transpileMethod(ns->methods[i], ctx);
	}
}

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileStructDecl(...)\n"); }
	
	fprintf(ctx->file ,"struct %s {\n", s->name);
	
	for(int i=0;i < s->count_members;i++){
		transpileStructMember(s->members[i], ctx);
	}
	
	fprintf(ctx->file, "};\n");
}

void transpileStructMember(struct StructMember* m, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileStructMember(...)\n"); }
	
	fprintf(ctx->file, "\t");
	
	transpileType(m->type, ctx);
	
	fprintf(ctx->file, " %s;\n", m->name);
}

void transpileMethod(struct Method* m, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileMethod(...)\n"); }

	transpileMethodSignature(m, ctx);

	transpileStmtBlock(m->block, ctx);
}

void transpileMethodSignature(struct Method* m, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileMethodSignature(...)\n"); }
	
	transpileType(m->returnType, ctx);

	fprintf(ctx->file, " %s(", m->name);

	for(int i=0; i < m->count_args; i++){
		transpileDeclArg(m->args[i], ctx);
		if(i < (m->count_args)-1){
			fprintf(ctx->file, ", ");
		}
	}

	fprintf(ctx->file, ")");
}

void transpileStmtBlock(struct StmtBlock* block, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileStmtBlock(...)\n"); }

	fprintf(ctx->file, "{\n");

	for(int i=0; i < block->count; i++){
		(ctx->indentLevel) += 1;
		transpileStmt(block->stmts[i], ctx);
		(ctx->indentLevel) -= 1;
	}

	indent(ctx);
	fprintf(ctx->file, "}\n");
}

void transpileStmt(struct Stmt* s, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileStmt(...)\n"); }

	if(s->m1 != NULL){
		transpileMethodCall(s->m1, ctx);
		fprintf(ctx->file, ";");
	}else if(s->m2 != NULL){
		transpileWhileStmt(s->m2, ctx);
	}else if(s->m3 != NULL){
		transpileIfStmt(s->m3, ctx);
	}else if(s->m4 != NULL){
		transpileRetStmt(s->m4, ctx);
		fprintf(ctx->file, ";");
	}else if(s->m5 != NULL){
		transpileAssignStmt(s->m5, ctx);
		fprintf(ctx->file, ";");
	}else{
		printf("Error in transpileStmt\n");
		exit(1);
	}
	fprintf(ctx->file, "\n");
}

//stmt related

void transpileMethodCall(struct MethodCall* mc, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileMethodCall(...)\n"); }
	
	indent(ctx);
	fprintf(ctx->file, "%s(", mc->methodName);

	for(int i=0;i < mc->count_args; i++){
		transpileExpr(mc->args[i], ctx);
		if(i < (mc->count_args)-1){
			fprintf(ctx->file, ", ");
		}
	}

	fprintf(ctx->file, ")");
}

void transpileWhileStmt(struct WhileStmt* ws, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileWhileStmt(...)\n"); }
	
	indent(ctx);
	
	fprintf(ctx->file, "while (");
	
	transpileExpr(ws->condition, ctx);
	
	fprintf(ctx->file, ")");

	transpileStmtBlock(ws->block, ctx);
}

void transpileIfStmt(struct IfStmt* is, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileIfStmt(...)\n"); }
	
	indent(ctx);
	
	fprintf(ctx->file, "if (");
	transpileExpr(is->condition, ctx);
	
	fprintf(ctx->file, ")");
		transpileStmtBlock(is->block, ctx);
	
	if(is->elseBlock != NULL){
		
		indent(ctx);
		fprintf(ctx->file, "else");
			transpileStmtBlock(is->elseBlock, ctx);
	}
}

void transpileRetStmt(struct RetStmt* rs, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileRetStmt(...)\n"); }
	
	indent(ctx);
	
	fprintf(ctx->file, "return ");
	transpileExpr(rs->returnValue, ctx);
}

void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileAssignStmt(...)\n"); }

	indent(ctx);

	if(as->optType != NULL){
		transpileType(as->optType, ctx);
		
		fprintf(ctx->file, " ");
	}
	
	transpileVariable(as->var, ctx);
	fprintf(ctx->file, " = ");
	transpileExpr(as->expr, ctx);
}

void transpileType(struct Type* t, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileType(...)\n"); }
	
	if(t->m1 != NULL){
		transpileBasicTypeWrapped(t->m1, ctx);
	}else if(t->m2 != NULL){
		transpileTypeParam(t->m2, ctx);
	}else if(t->m3 != NULL){
		transpileArrayType(t->m3, ctx);
	}
}

void transpileVariable(struct Variable* var, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileVariable(...)\n"); }
	
	transpileSimpleVar(var->simpleVar, ctx);
	
	for(int i=0; i < var->count_memberAccessList; i++){
		fprintf(ctx->file, ".");
		transpileVariable(var->memberAccessList[i], ctx);
	}
}

void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileUnOpTerm(...)\n"); }
	
	if(t->op != NULL){
		transpileOp(t->op, ctx);
	}
	transpileTerm(t->term, ctx);
}

void transpileTerm(struct Term* t, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileTerm(...)\n"); }
	
	if(t->m1 != NULL){
		transpileBoolConst(t->m1, ctx);
	}else if(t->m2 != NULL){
		transpileIntConst(t->m2, ctx);
	}else if(t->m3 != NULL){
		transpileCharConst(t->m3, ctx);
	}else if(t->m4 != NULL){
		transpileMethodCall(t->m4, ctx);
	}else if(t->m5 != NULL){
		transpileExpr(t->m5, ctx);
	}else if(t->m6 != NULL){
		transpileVariable(t->m6, ctx);
	}else if(t->m7 != NULL){
		transpileFloatConst(t->m7, ctx);
	}else if(t->m8 != NULL){
		transpileStringConst(t->m8, ctx);
	}else{
		printf("Error in transpileTerm\n");
		exit(1);
	}
}

void transpileExpr(struct Expr* expr, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileExpr(...)\n"); }

	transpileUnOpTerm(expr->term1, ctx);

	if(expr->op != NULL){
		transpileOp(expr->op, ctx);
		transpileUnOpTerm(expr->term2, ctx);
	}
}

void transpileSimpleVar(struct SimpleVar* svar, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileSimpleVar(...)\n"); }
	
	fprintf(ctx->file, "%s", svar->name);
	if(svar->optIndex != NULL){
		fprintf(ctx->file, "[");
		transpileExpr(svar->optIndex, ctx);
		fprintf(ctx->file, "]");
	}
}

void transpileBoolConst(struct BoolConst* bc, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileBoolConst(...)\n"); }
	
	if(bc->value){
		fprintf(ctx->file, "true");
	}else{
		fprintf(ctx->file, "false");
	}
}

void transpileIntConst(struct IntConst* ic, struct Ctx* ctx){
	fprintf(ctx->file, "%d", ic->value);
}

void transpileCharConst(struct CharConst* cc, struct Ctx* ctx){
	fprintf(ctx->file, "'%c'", cc->value);
}

void transpileFloatConst(struct FloatConst* fc, struct Ctx* ctx){
	fprintf(ctx->file, "%f", fc->value);
}

void transpileStringConst(struct StringConst* s, struct Ctx* ctx){
	//fprintf(ctx->file, "\"%s\"", s->value);
	
	//quotation seems to already be present
	fprintf(ctx->file, "%s", s->value);
}

void transpileOp(struct Op* op, struct Ctx* ctx){
	fprintf(ctx->file, " %s ", op->op);
}

void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileBasicTypeWrapped(...)\n"); }
	
	if(btw->simpleType != NULL){
		transpileSimpleType(btw->simpleType, ctx);
	}else if(btw->subrType != NULL){
		transpileSubrType(btw->subrType, ctx);
	}
}

void transpileTypeParam(struct TypeParam* tp, struct Ctx* ctx){
	//TODO
	printf("transpileTypeParam not yet implemented!\n");
	exit(1);
}

void transpileArrayType(struct ArrayType* atype, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileArrayType(...)\n"); }
	
	transpileType(atype->element_type, ctx);
	fprintf(ctx->file, "*");
}

void transpileSimpleType(struct SimpleType* simpleType, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileSimpleType(...)\n"); }
	
	//type name
	char* t = simpleType->typeName;
	char res[20];

	if(    strcmp(t, "PInt") == 0
		|| strcmp(t, "NInt") == 0
		|| strcmp(t, "Int") == 0
		|| strcmp(t, "NZInt") == 0  ){
		strcpy(res, "int");
	}else if(
		   strcmp(t, "PFloat") == 0
		|| strcmp(t, "NFloat") == 0
		|| strcmp(t, "Float") == 0
	){
		strcpy(res, "float");
	}else if( strcmp(t, "Bool") == 0 ){
		strcpy(res, "bool");
	}else if( strcmp(t, "String") == 0){
		strcpy(res, "char*");
	}else{
		//if we do not recognize it, treat it as struct pointer
		sprintf(res, "struct %s*", t);
	}

	fprintf(ctx->file, "%s", res);
}

void transpileSubrType(struct SubrType* subrType, struct Ctx* ctx){
	
	//https://www.zentut.com/c-tutorial/c-function-pointer/
	
	if(ctx->flags->debug){ printf("transpileSubrType(...)\n"); }

	//return type
	transpileType(subrType->returnType, ctx);

	//TODO: i do not really understand how
	//this is written in C. 
	//maybe trying some examples in C would help.
	fprintf(ctx->file, "(*function_ptr) (");

	//arguments
	for(int i=0; i < subrType->count_argTypes; i++){
		transpileType(subrType->argTypes[i], ctx);
		
		if(i < (subrType->count_argTypes)-1){
			fprintf(ctx->file, ", ");
		}
	}
	fprintf(ctx->file, ")");
}

void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileDeclArg(...)\n"); }
	
	transpileType(da->type, ctx);

	if(da->has_name){
		fprintf(ctx->file, " %s", da->name);
	}
}
