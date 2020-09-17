#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h> //for runtime assertions

#include "../../../ast/ast.h"
#include "../../../util/util.h"
#include "../../../ast/free_ast.h"
#include "../ctx.h"
#include "../flags.h"
#include "../typeinference.h"
#include "c_code_gen.h"
#include "code_gen_util.h"
#include "gen_c_types.h"
#include "structs_code_gen.h"

#include "../tables/localvarsymtable.h"
#include "../tables/subrsymtable.h"
#include "../tables/structsymtable.h"
#include "../tables/symtable.h"

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
void transpileForStmt(struct ForStmt* f, struct Ctx* ctx);
void transpileSwitchStmt(struct SwitchStmt* s, struct Ctx* ctx);
void transpileCaseStmt(struct CaseStmt* s, struct Ctx* ctx);
// --------------------
void transpileType(struct Type* t, struct Ctx* ctx);
void transpileVariable(struct Variable* var, struct Ctx* ctx);
void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx);
void transpileTerm(struct Term* expr, struct Ctx* ctx);
void transpileExpr(struct Expr* expr, struct Ctx* ctx);

void transpileSimpleVar(struct SimpleVar* svar, struct Ctx* ctx);

//const nodes related
void transpileBoolConst		(struct BoolConst* bc, 	struct Ctx* ctx);
void transpileIntConst		(struct IntConst* ic, 	struct Ctx* ctx);
void transpileCharConst		(struct CharConst* cc, 	struct Ctx* ctx);
void transpileFloatConst	(struct FloatConst* fc, struct Ctx* ctx);
void transpileStringConst	(struct StringConst* s, struct Ctx* ctx);
//-----------------------------
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
	ctx->flags = flags;
	ctx->indentLevel = 0;
	
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

	if(ctx->flags->stdout){
		ctx->file = stdout;
	}else{
		ctx->file = fopen(filename, "w");
	}

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
	
	transpileAST(ast, ctx);

	if(!(ctx->flags->stdout)){
		fclose(ctx->file);
	}
	
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
		fprintf(ctx->file, "#include <inttypes.h>\n");
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
	//and update the symbol table
	gen_struct_subrs_all(ns, ctx);
	
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
	
	bool isSubrType = false;
	//is it a function pointer?
	if(m->type->m1 != NULL){
		if(m->type->m1->subrType != NULL){
			isSubrType = true;
			strncpy(
				ctx->currentFunctionPointerVarOrArgName,
				m->name,
				DEFAULT_STR_SIZE
			);
		}
	}
	
	transpileType(m->type, ctx);
	
	if(!isSubrType){
		//with C function pointers, the identifier of the 
		//function pointer is between the types
		//describing it
		fprintf(ctx->file, " %s", m->name);
	}
	
	fprintf(ctx->file, ";\n");
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

	switch(s->kind){
		
		case 0:
			transpileLoopStmt(s->ptr.m0, ctx);
			break;
		
		case 1:
			indent(ctx);
			transpileMethodCall(s->ptr.m1, ctx);
			fprintf(ctx->file, ";");
			break;
		
		case 2:
			transpileWhileStmt(s->ptr.m2, ctx);
			break;
		
		case 3:
			transpileIfStmt(s->ptr.m3, ctx);
			break;
		
		case 4:
			transpileRetStmt(s->ptr.m4, ctx);
			fprintf(ctx->file, ";");
			break;
		
		case 5:
			transpileAssignStmt(s->ptr.m5, ctx);
			fprintf(ctx->file, ";");
			break;
		
		case 6:
			transpileBreakStmt(s->ptr.m6, ctx);
			break;
		
		case 7:
			transpileForStmt(s->ptr.m7, ctx);
			break;
		
		case 8:
			transpileSwitchStmt(s->ptr.m8, ctx);	
			break;
		
		default:
			printf("Error in transpileStmt\n");
			//still leaking memory, but less than before.
			//Due to includes lacking, not everything is freed here
			free(s);
			free(ctx);
			exit(1);
			break;
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
	
	//if we assign a function pointer
	bool isSubrType = false;

	if(as->optType != NULL){
		
		//is it a function pointer?
		if(as->optType->m1 != NULL){
			
			if(as->optType->m1->subrType != NULL){
				
				isSubrType = true;
				
				assert(as->var->simpleVar != NULL);
				
				strncpy(
					ctx->currentFunctionPointerVarOrArgName,
					
					//we know that it is a simple
					//variable (without index)because 
					//structures and arrays, 
					//would have no type
					//definition in front,
					//as they already have a known type
					as->var->simpleVar->name,
					DEFAULT_STR_SIZE
				);
			}
		}
		
		transpileType(as->optType, ctx);
		fprintf(ctx->file, " ");
		
	}else if(as->optType == NULL && as->var->count_memberAccessList == 0){
		//find type via local variable symbol table
		assert(ctx->tables->lvst != NULL);
		
		struct LVSTLine* line = lvst_get(
			ctx->tables->lvst, 
			as->var->simpleVar->name, 
			ctx->flags->debug
		);
		
		assert(line != NULL);
		
		if(line->firstOccur == as){
			
			//an assignment to this local variable first occurs in
			//this assignment statement
			transpileType(line->type, ctx);
			fprintf(ctx->file, " ");
		}
	}
	
	if(!isSubrType){
		//if it is a subroutine type,
		//in C unfortunately
		//the name of the variable is inbetween
		//the types
		transpileVariable(as->var, ctx);
	}
	
	fprintf(ctx->file, " %s ", as->assign_op);
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

void transpileForStmt(struct ForStmt* f, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileForStmt(...)\n"); }
	
	indent(ctx);
	
	fprintf(ctx->file, "for (");
	
	fprintf(ctx->file, "int %s = ", f->indexName);
	
	transpileExpr(f->range->start, ctx);
	
	fprintf(ctx->file, ";");
	
	fprintf(ctx->file, "%s <= ", f->indexName);
	
	transpileExpr(f->range->end, ctx);
	
	fprintf(ctx->file, "; %s++)", f->indexName);

	transpileStmtBlock(f->block, ctx);
}
void transpileSwitchStmt(struct SwitchStmt* s, struct Ctx* ctx){
	
	indent(ctx);
	fprintf(ctx->file, "switch (");
	transpileVariable(s->var, ctx);
	fprintf(ctx->file, ") {\n");
	
	(ctx->indentLevel) += 1;
	for(int i=0; i < s->count_cases; i++){
		transpileCaseStmt(s->cases[i], ctx);
	}
	(ctx->indentLevel) -= 1;
	
	indent(ctx);
	fprintf(ctx->file, "}\n");
}
void transpileCaseStmt(struct CaseStmt* s, struct Ctx* ctx){
	
	indent(ctx);
	fprintf(ctx->file, "case ");
	
	switch(s->kind){
		case 0: fprintf(ctx->file, "%s", (s->ptr.m1->value)?"true":"false"); break;
		case 1: fprintf(ctx->file, "'%c'", s->ptr.m2->value); break;
		case 2: fprintf(ctx->file, "%d", s->ptr.m3->value); break;
		default:
			printf("ERROR\n");
			exit(1);
	}
	fprintf(ctx->file, ":\n");
	
	if(s->block != NULL){
		indent(ctx);
		transpileStmtBlock(s->block, ctx);
		indent(ctx);
		fprintf(ctx->file, "break;\n");
	}
}
//-----------------------------------------------
void transpileType(struct Type* t, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileType(%p, %p)\n", t, ctx); }
	
	char* res = type2CType(t, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
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
	
	switch(t->kind){
		case 1:
			transpileBoolConst(t->ptr.m1, ctx);
			break;
		case 2:
			transpileIntConst(t->ptr.m2, ctx);
			break;
		case 3:
			transpileCharConst(t->ptr.m3, ctx);
			break;
		case 4:
			transpileMethodCall(t->ptr.m4, ctx);
			break;
		case 5:
			transpileExpr(t->ptr.m5, ctx);
			break;
		case 6:
			transpileVariable(t->ptr.m6, ctx);
			break;
		case 7:
			transpileFloatConst(t->ptr.m7, ctx);
			break;
		case 8:
			transpileStringConst(t->ptr.m8, ctx);
			break;
		default:
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
	
	for(int i=0;i < svar->count_indices; i++){
		fprintf(ctx->file, "[");
		transpileExpr(svar->indices[i], ctx);
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
	//quotation seems to already be present
	fprintf(ctx->file, "%s", s->value);
}

void transpileOp(struct Op* op, struct Ctx* ctx){
	fprintf(ctx->file, " %s ", op->op);
}

void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileBasicTypeWrapped(%p, %p)\n", btw, ctx); }
	
	char* res = basicTypeWrapped2CType(btw, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileTypeParam(struct TypeParam* tp, struct Ctx* ctx){
	
	if(ctx->flags->debug) { printf("transpileTypeParam(...)\n"); }
	
	char* res = typeParam2CType(tp, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileArrayType(struct ArrayType* atype, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileArrayType(...)\n"); }
	
	char* res = arrayType2CType(atype, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileSimpleType(struct SimpleType* simpleType, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileSimpleType(%p,%p)\n", simpleType, ctx); }
	
	char* res = simpleType2CType(simpleType);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileSubrType(struct SubrType* subrType, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileSubrType(...)\n"); }

	char* res = subrType2CType(subrType, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileDeclArg(...)\n"); }
	
	bool isSubrType = false;
	//is it a function pointer?
	if(da->type->m1 != NULL){
		if(da->type->m1->subrType != NULL){
			isSubrType = true;
			strncpy(
				ctx->currentFunctionPointerVarOrArgName,
				da->name,
				DEFAULT_STR_SIZE
			);
		}
	}
	
	transpileType(da->type, ctx);

	if(da->has_name && !isSubrType){
		//if it has a name, and is a subroutine type
		//(function pointer), then the name
		//is transpiled by transpileSubrType
		fprintf(ctx->file, " %s", da->name);
	}
}
