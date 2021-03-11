#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"

#include "util/ctx.h"
#include "flags/flags.h"

#include "typeinference/typeinfer.h"
#include "c_code_gen.h"
#include "code_gen_util.h"
#include "gen_c_types.h"
#include "structs_code_gen.h"

#include "analyzer/fn_analyzer.h"
#include "analyzer/lv_analyzer.h"

#include "tables/sst/sst.h"
#include "tables/sst/sst_prefill.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

// -------------------------------

//counter for generating labels
unsigned int label_count = 0;

//methods delcared in .c file, as they should be private
//to this file.

void transpileAST(struct AST* ast, struct Ctx* ctx);
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
void transpileBreakStmt(struct Ctx* ctx);
void transpileContinueStmt(struct Ctx* ctx);
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
void transpileHexConst		(struct HexConst* hc, 	struct Ctx* ctx);
void transpileBinConst		(struct BinConst* hc, 	struct Ctx* ctx);
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

bool transpileAndWrite(char* filename, struct AST* ast, struct Flags* flags){
	//returns false if it was unsuccessful
	
	assert(filename != NULL);
	assert(ast != NULL);
	assert(flags != NULL);
	
	if(flags->debug){ printf("transpileAndWrite(...)\n"); }

	struct Ctx* ctx = malloc(sizeof(struct Ctx));
	
	ctx->tables = makeST(flags->debug);
	
	ctx->flags = flags;
	
	ctx->indentLevel = 0;

	ctx->file = fopen(filename, "w");
	
	if(ctx->file == NULL){
		printf("could not open output file: %s\n", filename);
		
		freeST(ctx->tables);
		free(ctx);
		
		return false;
	}
	
	transpileAST(ast, ctx);

	fclose(ctx->file);
	
	freeST(ctx->tables);
	free(ctx);
	
	return true;
}

void transpileAST(struct AST* ast, struct Ctx* ctx){
	
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
	
	sst_clear(ctx->tables->sst);
	sst_prefill(ctx->tables, ctx->tables->sst);
	
	//TODO: re-enable
	//analyze_functions(ctx->tables, ast);

	for(int i=0; i < ast->count_namespaces; i++){

		transpileNamespace(ast->namespaces[i], ctx);
	}
}

void transpileNamespace(struct Namespace* ns, struct Ctx* ctx){
	
	if(ctx->flags->debug){ printf("transpileNamespace\n"); }
	
	sst_fill(ctx->tables->sst, ns, ctx->flags->debug);
	stst_fill(ctx->tables->stst, ns, ctx->flags->debug);
	
	ctx->flags->has_main_fn = sst_contains(ctx->tables->sst, "main");
	
	//write struct forward declarations
	assert(ns->structs != NULL);
	assert(ns->count_structs >= 0);
	
	for(int i=0;i < ns->count_structs; i++){
		assert(ns->structs[i] != NULL);
		
		fprintf(ctx->file, "struct %s;\n", ns->structs[i]->type->structType->typeName);
	}

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
}

void transpileStructDecl(struct StructDecl* s, struct Ctx* ctx){
	
	fprintf(ctx->file ,"struct %s {\n", s->type->structType->typeName);
	
	for(int i=0;i < s->count_members;i++){
		transpileStructMember(s->members[i], ctx);
	}
	
	fprintf(ctx->file, "};\n");
}

void transpileStructMember(struct StructMember* m, struct Ctx* ctx){
	
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
	
	if(ctx->flags->debug){ printf("transpileMethod\n"); }
	
	//create the local variable symbol table
	lvst_clear(ctx->tables->lvst);
	lvst_fill(m, ctx->tables, ctx->flags->debug);

	transpileMethodSignature(m, ctx);

	transpileStmtBlock(m->block, ctx);
}

void transpileMethodSignature(struct Method* m, struct Ctx* ctx){
	
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
	
	switch(s->kind){
		
		case 0: transpileLoopStmt(s->ptr.m0, ctx); break;
		
		case 1:
			indent(ctx);
			transpileMethodCall(s->ptr.m1, ctx);
			fprintf(ctx->file, ";");
			break;
		
		case 2: transpileWhileStmt(s->ptr.m2, ctx); break;
		
		case 3: transpileIfStmt(s->ptr.m3, ctx); break;
		
		case 4:
			transpileRetStmt(s->ptr.m4, ctx);
			fprintf(ctx->file, ";");
			break;
		
		case 5:
			transpileAssignStmt(s->ptr.m5, ctx);
			fprintf(ctx->file, ";");
			break;
		
		case 7: transpileForStmt(s->ptr.m7, ctx); break;
		
		case 8: transpileSwitchStmt(s->ptr.m8, ctx); break;

		case 99:
			if(s->isBreak){ transpileBreakStmt(ctx);  }
			if(s->isContinue){ transpileContinueStmt(ctx); }
			break;
		
		default:
			printf("Error in transpileStmt\n");
			exit(1);
	}
	
	fprintf(ctx->file, "\n");
}

//stmt related

void transpileMethodCall(struct MethodCall* mc, struct Ctx* ctx){
	
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
	
	indent(ctx);
	
	fprintf(ctx->file, "while (");
	
	transpileExpr(ws->condition, ctx);
	
	fprintf(ctx->file, ")");

	transpileStmtBlock(ws->block, ctx);
}

void transpileIfStmt(struct IfStmt* is, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "if (");
	transpileExpr(is->condition, ctx);
	
	fprintf(ctx->file, ")");
	transpileStmtBlock(is->block, ctx);
	
	if(is->elseBlock == NULL){ return; }
	
	indent(ctx);
	fprintf(ctx->file, "else");
	transpileStmtBlock(is->elseBlock, ctx);
}

void transpileRetStmt(struct RetStmt* rs, struct Ctx* ctx){
	
	indent(ctx);
	
	fprintf(ctx->file, "return ");
	transpileExpr(rs->returnValue, ctx);
}

void transpileAssignStmt(struct AssignStmt* as, struct Ctx* ctx){

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
		
	}else if(as->optType == NULL && as->var->memberAccess == NULL){
		//find type via local variable symbol table
		assert(ctx->tables->lvst != NULL);
		
		struct LVSTLine* line = lvst_get(
			ctx->tables->lvst, as->var->simpleVar->name
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
		//if it is a subroutine type, in C unfortunately
		//the name of the variable is inbetween the types.
		transpileVariable(as->var, ctx);
	}
	
	fprintf(ctx->file, " %s ", as->assign_op);
	transpileExpr(as->expr, ctx);
}

void transpileLoopStmt(struct LoopStmt* ls, struct Ctx* ctx){

	indent(ctx);
	
	unsigned int i = label_count++;
	fprintf(ctx->file, "int count%d = ", i);
	
	transpileExpr(ls->count, ctx);
	
	fprintf(ctx->file, ";\n");
	
	indent(ctx);
	
	fprintf(ctx->file, "while (count%d-- > 0)", i);

	transpileStmtBlock(ls->block, ctx);
}

void transpileBreakStmt(struct Ctx* ctx){
	indent(ctx); fprintf(ctx->file, "break;");
}

void transpileContinueStmt(struct Ctx* ctx){
	indent(ctx); fprintf(ctx->file, "continue;");
}

void transpileForStmt(struct ForStmt* f, struct Ctx* ctx){
		
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
			printf("ERROR\n"); exit(1);
	}
	fprintf(ctx->file, ":\n");
	
	if(s->block == NULL){ return; }
	
	indent(ctx);
	transpileStmtBlock(s->block, ctx);
	indent(ctx);
	fprintf(ctx->file, "break;\n");
}
//-----------------------------------------------
void transpileType(struct Type* t, struct Ctx* ctx){
	
	assert(t != NULL);
	
	if(t->m1 != NULL)
		{ transpileBasicTypeWrapped(t->m1, ctx); }
	
	if(t->m2 != NULL)
		{ transpileTypeParam(t->m2, ctx); }
	
	if(t->m3 != NULL)
		{ transpileArrayType(t->m3, ctx); }
}

void transpileVariable(struct Variable* var, struct Ctx* ctx){
		
	transpileSimpleVar(var->simpleVar, ctx);
	
	if(var->memberAccess != NULL){
		fprintf(ctx->file, "->");
		transpileVariable(var->memberAccess, ctx);
	}
}

void transpileUnOpTerm(struct UnOpTerm* t, struct Ctx* ctx){
		
	if(t->op != NULL){ transpileOp(t->op, ctx); }
	
	transpileTerm(t->term, ctx);
}

void transpileTerm(struct Term* t, struct Ctx* ctx){
		
	switch(t->kind){
		
		case 1: transpileBoolConst(t->ptr.m1, ctx); break;
		case 2: transpileIntConst(t->ptr.m2, ctx); break;
		case 3: transpileCharConst(t->ptr.m3, ctx); break;
		case 4: transpileMethodCall(t->ptr.m4, ctx); break;
		case 5: 
			fprintf(ctx->file, "(");
			transpileExpr(t->ptr.m5, ctx); 
			fprintf(ctx->file, ")");
			break;
		case 6: transpileVariable(t->ptr.m6, ctx); break;
		case 7: transpileFloatConst(t->ptr.m7, ctx); break;
		case 8: transpileStringConst(t->ptr.m8, ctx); break;
		case 9: transpileHexConst(t->ptr.m9, ctx); break;
		case 10:transpileBinConst(t->ptr.m10, ctx); break;
		
		default:
			printf("[Transpiler][Error] in transpileTerm\n"); exit(1);
	}
}

void transpileExpr(struct Expr* expr, struct Ctx* ctx){
	
	assert(expr != NULL);
	
	transpileUnOpTerm(expr->term1, ctx);

	if(expr->op != NULL){
		transpileOp(expr->op, ctx);
		transpileUnOpTerm(expr->term2, ctx);
	}
}

void transpileSimpleVar(struct SimpleVar* svar, struct Ctx* ctx){
		
	assert(svar->name != NULL);
	
	fprintf(ctx->file, "%s", svar->name);
	
	for(int i=0;i < svar->count_indices; i++){
		fprintf(ctx->file, "[");
		transpileExpr(svar->indices[i], ctx);
		fprintf(ctx->file, "]");
	}
}

void transpileBoolConst(struct BoolConst* bc, struct Ctx* ctx){
	fprintf(ctx->file, (bc->value)?"true":"false");
}

void transpileIntConst(struct IntConst* ic, struct Ctx* ctx){
	fprintf(ctx->file, "%d", ic->value);
}

void transpileHexConst(struct HexConst* hc, struct Ctx* ctx){
	fprintf(ctx->file, "0x%x", hc->value);
}

void transpileBinConst(struct BinConst* hc, struct Ctx* ctx){
	
	fprintf(ctx->file, "0b");
	
	uint32_t value = hc->value;
	
	if(value == 0){
		fprintf(ctx->file, "0");
		return;
	}
	
	const int size = 128;
	
	char buffer[size];
	
	int index = size - 1;
	buffer[index--] = '\0';
	
	while(value > 0){
		
		uint8_t bit = value & 0x1;
		
		buffer[index--] = (bit == 0x1) ? '1' : '0';
		
		value >>= 1;
	}
	
	fprintf(ctx->file, "%s", buffer+index+1);
}

void transpileCharConst(struct CharConst* cc, struct Ctx* ctx){
	fprintf(ctx->file, "'%c'", cc->value);
}

void transpileFloatConst(struct FloatConst* fc, struct Ctx* ctx){
	fprintf(ctx->file, "%f", fc->value);
}

void transpileStringConst(struct StringConst* s, struct Ctx* ctx){
	fprintf(ctx->file, "%s", s->value);
}

void transpileOp(struct Op* op, struct Ctx* ctx){
	fprintf(ctx->file, " %s ", op->op);
}

void transpileBasicTypeWrapped(struct BasicTypeWrapped* btw, struct Ctx* ctx){
		
	assert(btw != NULL);
		
	char* res = basicTypeWrapped2CType(btw, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileTypeParam(struct TypeParam* tp, struct Ctx* ctx){
		
	char* res = typeParam2CType(tp, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileArrayType(struct ArrayType* atype, struct Ctx* ctx){
		
	char* res = arrayType2CType(atype, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileSimpleType(struct SimpleType* simpleType, struct Ctx* ctx){
		
	char* res = simpleType2CType(simpleType);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileSubrType(struct SubrType* subrType, struct Ctx* ctx){
	
	char* res = subrType2CType(subrType, ctx);
	fprintf(ctx->file, "%s", res);
	free(res);
}

void transpileDeclArg(struct DeclArg* da, struct Ctx* ctx){
		
	assert(da != NULL);
		
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
