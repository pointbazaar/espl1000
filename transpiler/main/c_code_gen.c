#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h> //for runtime assertions

#include "c_code_gen.h"
#include "ctx.h"
#include "code_gen_util.h"
#include "../../util/util.h"
#include "../../ast/free_ast.h"
#include "typeinference.h"
#include "structs_code_gen.h"

#include "tables/localvarsymtable.h"
#include "tables/subrsymtable.h"
#include "tables/structsymtable.h"
#include "tables/symtable.h"

// -------------------------------

//counter for generating labels
unsigned int label_count = 0;

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
void transpileLoopStmt(struct LoopStmt* ls, struct Ctx* ctx);
void transpileBreakStmt(struct BreakStmt* ls, struct Ctx* ctx);

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

// --------------------------------------------------------
// --------------------------------------------------------

bool transpileAndWrite(char* filename, struct AST_Whole_Program* ast, struct Flags* flags){
	//returns false if it was unsuccessful
	
	assert(filename != NULL);
	assert(ast != NULL);
	assert(flags != NULL);
	
	if(flags->debug){ printf("transpileAndWrite(...)\n"); }

	struct Ctx* ctx = smalloc(sizeof(struct Ctx));
	ctx->tables = smalloc(sizeof(struct ST));
	
	//100 should be enough for now,
	//if it is more, it will exit and print error.
	ctx->tables->inferredTypesCapacity = 100;
	ctx->tables->inferredTypesCount = 0;
	ctx->tables->inferredTypes = 
		smalloc(
			sizeof(struct Type*) 
			* ctx->tables->inferredTypesCapacity
		);

	if(flags->debug){
		printf("SET ctx->file\n");
	}

	ctx->file = fopen(filename, "w");

	if(ctx->file == NULL){
		printf("could not open output file: %s\n", filename);
		
		free(ctx->tables);
		free(ctx);
		
		return false;
	}
	
	if(flags->debug){
		printf("SET ctx->flags\n");
		printf("SET ctx->indentLevel\n");
	}
	
	ctx->flags = flags;
	ctx->indentLevel = 0;

	transpileAST(ast, ctx);

	fclose(ctx->file);
	
	//free ctx struct
	for(int i = 0; i < ctx->tables->inferredTypesCount; i++){
		freeType(ctx->tables->inferredTypes[i]);
	}
	free(ctx->tables->inferredTypes);
	free(ctx->tables);
	free(ctx);
	
	if(flags->debug){ printf("transpileAndWrite(...) DONE\n"); }
	return true;
}

void transpileAST(struct AST_Whole_Program* ast, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileAST(...)\n"); }
	
	//write some standard stdlib includes
	
	if(!(ctx->flags->avr)){
		//in microcontrollers, we cannot assume there will
		//be stdlib 
		
		fprintf(ctx->file, "#include <stdlib.h>\n");
		fprintf(ctx->file, "#include <stdio.h>\n");
		fprintf(ctx->file, "#include <stdbool.h>\n");
		fprintf(ctx->file, "#include <string.h>\n");
		fprintf(ctx->file, "#include <math.h>\n");
	}

	for(int i=0; i < ast->count_namespaces; i++){

		transpileNamespace(ast->namespaces[i], ctx);
	}
}

void transpileNamespace(struct Namespace* ns, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileNamespace(...)\n"); }
	
	if(ctx->flags->debug){ printf("SET ctx->tables->sst\n"); }
	ctx->tables->sst = makeSubrSymTable(ns, ctx->flags->debug);
	
	if(ctx->flags->debug){ printf("SET ctx->tables->stst\n"); }
	ctx->tables->stst = makeStructSymTable(ns, ctx->flags->debug);
	
	//lvst gets populated later
	ctx->tables->lvst = NULL;
	
	//write struct forward declarations
	assert(ns->structs != NULL);
	assert(ns->count_structs >= 0);
	
	for(int i=0;i < ns->count_structs; i++){
		assert(ns->structs[i] != NULL);
		
		fprintf(ctx->file, "struct %s;\n", ns->structs[i]->name);
	}

	//transpile struct definitions
	for(int i=0;i < ns->count_structs; i++){
		transpileStructDecl(ns->structs[i], ctx);
	}
	
	//generate the struct specific
	//constructors, destructors,
	//copy-constructors
	gen_struct_subr_signatures(ns, ctx);
	gen_struct_subrs(ns, ctx);
	
	//write subroutine forward declarations
	for(int i=0; i < ns->count_methods; i++){
		transpileMethodSignature(ns->methods[i], ctx);
		fprintf(ctx->file, ";\n");
	}

	for(int i=0; i < ns->count_methods; i++){
		transpileMethod(ns->methods[i], ctx);
	}
	
	freeSubrSymTable(ctx->tables->sst);
	ctx->tables->sst = NULL;
	freeStructSymTable(ctx->tables->stst);
	ctx->tables->stst = NULL;
}

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileStructDecl(%p,%p)\n", s, ctx); }
	
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
	
	if(ctx->flags->debug){ printf("transpileMethod(%p, %p)\n", m, ctx); }
	
	//create the local variable symbol table
	ctx->tables->lvst = makeLocalVarSymTable(ctx->flags->debug);
	fillLocalVarSymTable(m, ctx->tables, ctx->flags->debug);

	transpileMethodSignature(m, ctx);

	transpileStmtBlock(m->block, ctx);
	
	freeLocalVarSymTable(ctx->tables->lvst);
	ctx->tables->lvst = NULL;
}

void transpileMethodSignature(struct Method* m, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileMethodSignature(%p, %p)\n", m, ctx); }
	
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

	if(s->m0 != NULL){
		transpileLoopStmt(s->m0, ctx);
		
	}else if(s->m1 != NULL){
		indent(ctx);
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
		
	}else if(s->m6 != NULL){
		transpileBreakStmt(s->m6, ctx);
		
	}else{
		printf("Error in transpileStmt\n");
		//still leaking memory, but less than before.
		//Due to includes lacking, not everything is freed here
		free(s);
		free(ctx);
		exit(1);
	}
	fprintf(ctx->file, "\n");
}

//stmt related

void transpileMethodCall(struct MethodCall* mc, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileMethodCall(...)\n"); }
	
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
	
	if(ctx->flags->debug){ 
		printf("transpileAssignStmt(%p, %p)\n", as, ctx); 
	}

	indent(ctx);

	if(as->optType != NULL){
		
		transpileType(as->optType, ctx);
		
	}else{
		//find type via local variable symbol table
		assert(ctx->tables->lvst != NULL);
		
		struct LVSTLine* line = lvst_get(ctx->tables->lvst, as->var->simpleVar->name);
		
		assert(line != NULL);
		
		if(line->firstOccur == as){
			
			//an assignment to this local variable first occurs in
			//this assignment statement
			transpileType(line->type, ctx);
		}
	}
	
	fprintf(ctx->file, " ");
	
	transpileVariable(as->var, ctx);
	fprintf(ctx->file, " = ");
	transpileExpr(as->expr, ctx);
}

void transpileLoopStmt(struct LoopStmt* ls, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileLoopStmt(...)\n"); }
	
	indent(ctx);
	
	unsigned int i = label_count++;
	fprintf(ctx->file, "int count%d = ", i);
	
	transpileExpr(ls->count, ctx);
	
	fprintf(ctx->file, ";\n");
	
	indent(ctx);
	
	fprintf(ctx->file, "while (");
	
	fprintf(ctx->file, "count%d-- > 0", i);
	
	fprintf(ctx->file, ")");

	transpileStmtBlock(ls->block, ctx);
}

void transpileBreakStmt(struct BreakStmt* ls, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileBreakStmt(...)\n"); }
	indent(ctx);
	
	fprintf(ctx->file, "break;");
}

void transpileType(struct Type* t, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileType(%p, %p)\n", t, ctx); }
	
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
		fprintf(ctx->file, "->");
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
	
	if(ctx->flags->debug){ printf("transpileSimpleVar(%p, %p)\n", svar, ctx); }
	
	assert(svar->name != NULL);
	
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
	
	if(ctx->flags->debug){ printf("transpileBasicTypeWrapped(%p, %p)\n", btw, ctx); }
	
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
	
	if(ctx->flags->debug){ printf("transpileSimpleType(%p,%p)\n", simpleType, ctx); }
	
	//type name
	char* t = simpleType->typeName;
	char* res = malloc(sizeof(char)*DEFAULT_STR_SIZE*3);
	strcpy(res, "");

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
		
	}else if( strcmp(t, "Char") == 0){
		strcpy(res, "char");
		
	}else{
		//if we do not recognize it, treat it as struct pointer
		sprintf(res, "struct %s*", t);
	}
	
	fprintf(ctx->file, "%s", res);
	free(res);
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
