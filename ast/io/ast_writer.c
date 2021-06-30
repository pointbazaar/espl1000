#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "ast_writer.h"
#include "magic_num.h"
#include "serialize.h"

#define structsize(X) sizeof(struct X)

#define write_super(X) serialize_astnode(&(X->super), file)

static void error(FILE* file, char* msg){
	
	if(file != NULL){ fclose(file); }
	
	printf("[AST][Error] %s\n", msg);
	exit(1);
}

static int opt2int(void* opt){
	return (opt == NULL)?OPT_EMPTY:OPT_PRESENT;
}

void writeAST(struct AST* ast){
	
	for(int i = 0; i < ast->count_namespaces; i++)
		{ writeNamespace(ast->namespaces[i]); }
}

void writeNamespace(struct Namespace* n){
	
	FILE* file = fopen(n->ast_path, "w");

	if(file == NULL){
		
		printf("%s\n", n->ast_path);
		error(NULL, "could not open file");
	}
	
	magic_num_serialize(MAGIC_NAMESPACE, file);
	
	serialize_string(n->src_path, file);
	serialize_string(n->ast_path, file);
	serialize_string(n->name,     file);
	
	serialize_int(n->count_methods, file);
	
	for(int i=0;i < n->count_methods;i++)
		{ writeMethod(n->methods[i], file); }
	
	serialize_int(n->count_structs, file);
	
	for(int i=0;i < n->count_structs;i++)
		{ writeStructDecl(n->structs[i], file); }
	
	magic_num_serialize(MAGIC_END_NAMESPACE, file);
	
	fclose(file);
	
}
void writeMethod(struct Method* m, FILE* file){
	
	magic_num_serialize(MAGIC_METHOD, file);
	
	write_super(m);
	
	serialize_int(m->isPublic, file);
	serialize_int(m->hasSideEffects, file);
	
	serialize_string(m->name, file);
	
	writeType(m->returnType,file);

	serialize_int(m->count_args, file);
	
	for(int i = 0;i < m->count_args;i++)
		{ writeDeclArg(m->args[i], file); }

	writeStmtBlock(m->block, file);
	
	magic_num_serialize(MAGIC_END_METHOD, file);
}
void writeStructDecl(struct StructDecl* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRUCTDECL, file);
	
	write_super(m);
	
	writeSimpleType(m->type, file);
	
	serialize_int(m->count_members, file);
	
	for(int i = 0;i < m->count_members;i++)
		{ writeStructMember(m->members[i], file); }
	
	magic_num_serialize(MAGIC_END_STRUCTDECL, file);
}
void writeStructMember(struct StructMember* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRUCTMEMBER, file);
	
	write_super(m);
	
	writeType(m->type, file);
	
	serialize_string(m->name, file);
	
	magic_num_serialize(MAGIC_END_STRUCTMEMBER, file);
}

void writeStmtBlock(struct StmtBlock* block, FILE* file){
	
	magic_num_serialize(MAGIC_STMTBLOCK, file);
	
	write_super(block);
	
	serialize_int(block->count, file);
	
	for(int i = 0;i < block->count; i++)
		{ writeStmt(block->stmts[i], file); }
	
	magic_num_serialize(MAGIC_END_STMTBLOCK, file);
}

void writeDeclArg(struct DeclArg* m, FILE* file){

	magic_num_serialize(MAGIC_DECLARG, file);
	
	write_super(m);

	writeType(m->type,file);
	
	serialize_int(opt2int(m->name), file);
	
	if(m->name != NULL)
		{ serialize_string(m->name, file); }
	
	magic_num_serialize(MAGIC_END_DECLARG, file);
}
void writeVariable(struct Variable* m, FILE* file){

	magic_num_serialize(MAGIC_VARIABLE, file);
	
	write_super(m);

	writeSimpleVar(m->simpleVar,file);

	serialize_int(opt2int(m->memberAccess), file);
	
	if(m->memberAccess != NULL)
		{ writeVariable(m->memberAccess, file); }

	magic_num_serialize(MAGIC_END_VARIABLE, file);
}
void writeSimpleVar(struct SimpleVar* m, FILE* file){

	magic_num_serialize(MAGIC_SIMPLEVAR, file);
	
	write_super(m);

	serialize_string(m->name, file);

	serialize_int(m->count_indices, file);
	
	for(int i=0;i < m->count_indices; i++)
		{ writeExpr(m->indices[i], file); }
	
	magic_num_serialize(MAGIC_END_SIMPLEVAR, file);
}
void writeExpr(struct Expr* m, FILE* file){

	magic_num_serialize(MAGIC_EXPR, file);
	
	write_super(m);

	writeUnOpTerm(m->term1, file);
	
	serialize_int(opt2int(m->op), file);
	
	if(m->op != NULL){
		
		writeOp(m->op,file);
		writeUnOpTerm(m->term2,file);
	}
	
	magic_num_serialize(MAGIC_END_EXPR, file);
}
void writeTerm(struct Term* m, FILE* file){
	
	magic_num_serialize(MAGIC_TERM, file);
	
	write_super(m);

	serialize_int(m->kind, file);

	switch(m->kind){
		case  1: writeBoolConst(m->ptr.m1,file);  break;
		case  2: writeIntConst(m->ptr.m2,file);   break;
		case  3: writeCharConst(m->ptr.m3,file);  break;
		case  4: writeCall(m->ptr.m4,file); break;
		case  5: writeExpr(m->ptr.m5,file);       break;
		case  6: writeVariable(m->ptr.m6,file);   break;
		case  7: writeFloatConst(m->ptr.m7,file); break;
		case  8: writeStringConst(m->ptr.m8, file); break;
		case  9: writeHexConst(m->ptr.m9, file);  break;
		case 10: writeBinConst(m->ptr.m10, file); break;
		case 11: writeLambda(m->ptr.m11, file);	  break;
		default: error(file, "Error in writeTerm(...)");
	}
	
	magic_num_serialize(MAGIC_END_TERM, file);
}
void writeUnOpTerm(struct UnOpTerm* t, FILE* file){
	
	magic_num_serialize(MAGIC_UNOPTERM, file);
	
	write_super(t);
	
	serialize_int(opt2int(t->op), file);
	
	if(t->op != NULL){ writeOp(t->op, file); }
	
	writeTerm(t->term, file);
	
	magic_num_serialize(MAGIC_END_UNOPTERM, file);
}
void writeRange(struct Range* r, FILE* file){
	
	magic_num_serialize(MAGIC_RANGE, file);
	
	write_super(r);
	
	writeExpr(r->start, file);
	writeExpr(r->end, file);
	
	magic_num_serialize(MAGIC_END_RANGE, file);
}
void writeLambda(struct Lambda* l, FILE* file){
	
	magic_num_serialize(MAGIC_LAMBDA, file);
	
	write_super(l);
	
	serialize_int(l->count_params, file);
	
	for(uint8_t i = 0; i < l->count_params; i++){
		serialize_string(l->params[i]->identifier, file);
	}
	
	writeExpr(l->expr, file);
	
	magic_num_serialize(MAGIC_END_LAMBDA, file);
}
// --------- CONST NODES ----------------
void writeBoolConst(struct BoolConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_BOOLCONST, file);
	write_super(m);
	fwrite(m, structsize(BoolConst), 1, file);
	magic_num_serialize(MAGIC_END_BOOLCONST, file);
}
void writeIntConst(struct IntConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_INTCONST, file);
	write_super(m);
	fwrite(m, structsize(IntConst), 1, file);
	magic_num_serialize(MAGIC_END_INTCONST, file);
}
void writeHexConst(struct HexConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_HEXCONST, file);
	write_super(m);
	fwrite(m, structsize(HexConst), 1, file);
	magic_num_serialize(MAGIC_END_HEXCONST, file);
}
void writeBinConst(struct BinConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_BINCONST, file);
	write_super(m);
	fwrite(m, structsize(BinConst), 1, file);
	magic_num_serialize(MAGIC_END_BINCONST, file);
}
void writeCharConst(struct CharConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_CHARCONST, file);
	write_super(m);
	fwrite(m, structsize(CharConst), 1, file);
	magic_num_serialize(MAGIC_END_CHARCONST, file);
}
void writeFloatConst(struct FloatConst* m, 	FILE* file){
	
	magic_num_serialize(MAGIC_FLOATCONST, file);
	write_super(m);
	fwrite(m, structsize(FloatConst), 1, file);
	magic_num_serialize(MAGIC_END_FLOATCONST, file);
}
void writeStringConst(struct StringConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRINGCONST, file);
	write_super(m);
	serialize_string(m->value, file);
	
	magic_num_serialize(MAGIC_END_STRINGCONST, file);
}
void writeOp(struct Op* m, FILE* file){
	
	magic_num_serialize(MAGIC_OP, file);
	write_super(m);
	fwrite(m, structsize(Op), 1, file);
	
	magic_num_serialize(MAGIC_END_OP, file);
}
// ---------------- STATEMENTNODES ---------------------
void writeStmt(struct Stmt* m, FILE* file){

	magic_num_serialize(MAGIC_STMT, file);
	write_super(m);
	serialize_int(m->kind, file);
	
	switch(m->kind){
		case 99: 
			{
				serialize_int( m->isBreak ? OPT_PRESENT : OPT_EMPTY, file);
				serialize_int( m->isContinue ? OPT_PRESENT : OPT_EMPTY, file);
			}
			break;
		case 0: { writeLoopStmt(m->ptr.m0, file);  } break;
		case 1: { writeCall(m->ptr.m1,file); } break;
		case 2: { writeWhileStmt(m->ptr.m2,file);  } break;
		case 3: { writeIfStmt(m->ptr.m3,file);     } break;
		case 4: { writeRetStmt(m->ptr.m4,file);    } break;
		case 5: { writeAssignStmt(m->ptr.m5,file); } break;
		case 7: { writeForStmt(m->ptr.m7,file);    } break;
		case 8: { writeSwitchStmt(m->ptr.m8,file); } break;
		default: error(file, "Error in writeStmt");
	}
	
	magic_num_serialize(MAGIC_END_STMT, file);
}

void writeIfStmt(struct IfStmt* m, FILE* file){

	magic_num_serialize(MAGIC_IFSTMT, file);
	write_super(m);
	writeExpr(m->condition, file);
	writeStmtBlock(m->block, file);
	
	serialize_int(opt2int(m->elseBlock), file);
	
	if(m->elseBlock != NULL)
		{ writeStmtBlock(m->elseBlock, file); }
	
	magic_num_serialize(MAGIC_END_IFSTMT, file);
}

void writeWhileStmt(struct WhileStmt* m, FILE* file){

	magic_num_serialize(MAGIC_WHILESTMT, file);
	write_super(m);
	writeExpr(m->condition, file);
	writeStmtBlock(m->block, file);
	
	magic_num_serialize(MAGIC_END_WHILESTMT, file);
}

void writeRetStmt(struct RetStmt* m, FILE* file){

	magic_num_serialize(MAGIC_RETSTMT, file);
	write_super(m);
	writeExpr(m->returnValue,file);
	
	magic_num_serialize(MAGIC_END_RETSTMT, file);
}

void writeAssignStmt(struct AssignStmt* m, FILE* file){

	magic_num_serialize(MAGIC_ASSIGNSTMT, file);
	write_super(m);
	serialize_int(opt2int(m->optType), file);

	if(m->optType != NULL){
		writeType(m->optType, file);
	}
	
	writeVariable(m->var, file);
	
	serialize_string(m->assign_op, file);
	
	writeExpr(m->expr, file);
	
	magic_num_serialize(MAGIC_END_ASSIGNSTMT, file);
}
void writeCall(struct Call* m, FILE* file){

	magic_num_serialize(MAGIC_CALL, file);
	write_super(m);
	serialize_string(m->name, file);
	serialize_int(m->count_args, file);
	
	for(int i=0;i < m->count_args;i++)
		{ writeExpr(m->args[i],file); }
	
	magic_num_serialize(MAGIC_END_CALL, file);
}
void writeLoopStmt(struct LoopStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_LOOPSTMT, file);
	write_super(m);
	writeExpr(m->count, file);
	writeStmtBlock(m->block, file);
	
	magic_num_serialize(MAGIC_END_LOOPSTMT, file);
}
void writeForStmt(struct ForStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_FORSTMT, file);
	write_super(m);
	serialize_string(m->indexName, file);

	writeRange(m->range, file);
	writeStmtBlock(m->block, file);
	
	magic_num_serialize(MAGIC_END_FORSTMT, file);
}
void writeSwitchStmt(struct SwitchStmt* m,	FILE* file){
	
	magic_num_serialize(MAGIC_SWITCHSTMT, file);
	write_super(m);
	writeExpr(m->expr, file);
	
	serialize_int(m->count_cases, file);
	
	for(int i=0; i < m->count_cases; i++)
		{ writeCaseStmt(m->cases[i], file); }
	
	magic_num_serialize(MAGIC_END_SWITCHSTMT, file);
}
void writeCaseStmt(struct CaseStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_CASESTMT, file);
	write_super(m);
	serialize_int(m->kind, file);
	
	switch(m->kind){
		case 0: writeBoolConst(m->ptr.m1, file); break;
		case 1: writeCharConst(m->ptr.m2, file); break;
		case 2: writeIntConst(m->ptr.m3, file); break;
		default: error(file, "kind has wrong value");
	}
	
	serialize_int(opt2int(m->block), file);
	
	if(m->block != NULL)
		{ writeStmtBlock(m->block, file); }
	
	magic_num_serialize(MAGIC_END_CASESTMT, file);
}
// --------- TYPENODES --------------
void writeType(struct Type* m, FILE* file){
	
	magic_num_serialize(MAGIC_TYPE, file);
	write_super(m);
	if(m->m1 != NULL){
		serialize_int(1, file);
		writeBasicTypeWrapped(m->m1,file);

	}else if(m->m2 != NULL){
		serialize_int(2, file);
		writeTypeParam(m->m2,file);

	}else if(m->m3 != NULL){
		serialize_int(3, file);
		writeArrayType(m->m3,file);
	}
	
	magic_num_serialize(MAGIC_END_TYPE, file);
}

void writeArrayType(struct ArrayType* m, FILE* file){
	
	magic_num_serialize(MAGIC_ARRAYTYPE, file);
	write_super(m);
	writeType(m->element_type,file);
	
	magic_num_serialize(MAGIC_END_ARRAYTYPE, file);
}

void writeTypeParam(struct TypeParam* m, FILE* file){
	
	magic_num_serialize(MAGIC_TYPEPARAM, file);
	write_super(m);
	fwrite(m, structsize(TypeParam), 1, file);
	
	magic_num_serialize(MAGIC_END_TYPEPARAM, file);
}

void writeBasicTypeWrapped(struct BasicTypeWrapped* m, FILE* file){

	magic_num_serialize(MAGIC_BASICTYPEWRAPPED, file);
	write_super(m);
	if(m->simpleType != NULL){ 
		
		serialize_int(1, file);
		writeSimpleType(m->simpleType,file); 
		
	}else if(m->subrType != NULL){ 
		
		serialize_int(2, file);
		writeSubrType(m->subrType,file); 
		
	}
	
	magic_num_serialize(MAGIC_END_BASICTYPEWRAPPED, file);
}

void writeStructType(struct StructType* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRUCTTYPE, file);
	write_super(m);
	serialize_string(m->typeName, file);
	serialize_int(m->typeParamCount, file);
	
	for(int i = 0;i < m->typeParamCount;i++)
		{ serialize_int(m->typeParams[i], file); }
	
	magic_num_serialize(MAGIC_END_STRUCTTYPE, file);
}

void writePrimitiveType(struct PrimitiveType* m, FILE* file){
	
	magic_num_serialize(MAGIC_PRIMITIVETYPE, file);
	write_super(m);
	
	serialize_int(m->isIntType,   file);
	serialize_int(m->isFloatType, file);
	serialize_int(m->isCharType,  file);
	serialize_int(m->isBoolType,  file);

	serialize_int(m->intType, file);

	magic_num_serialize(MAGIC_END_PRIMITIVETYPE, file);
}

void writeSimpleType(struct SimpleType* m, FILE* file){
	
	magic_num_serialize(MAGIC_SIMPLETYPE, file);
	write_super(m);
	
	serialize_int(opt2int(m->structType), file);
	
	if(m->primitiveType != NULL){
		writePrimitiveType(m->primitiveType, file);
	}
	
	if(m->structType != NULL){
		writeStructType(m->structType, file);
	}
	
	magic_num_serialize(MAGIC_END_SIMPLETYPE, file);
}

void writeSubrType(struct SubrType* m, FILE* file){

	magic_num_serialize(MAGIC_SUBRTYPE, file);
	write_super(m);
	
	writeType(m->returnType,file);
	serialize_int(m->hasSideEffects, file);

	serialize_int(m->count_argTypes, file);
	
	for(int i = 0;i < m->count_argTypes;i++)
		{ writeType(m->argTypes[i], file); }
	
	magic_num_serialize(MAGIC_END_SUBRTYPE, file);
}
