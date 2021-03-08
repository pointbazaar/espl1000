#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "ast_writer.h"
#include "magic_num.h"
#include "serialize.h"


void writeNamespace(struct Namespace* nsn, FILE* file){
	
	magic_num_serialize(MAGIC_NAMESPACE, file);
	
	serialize_string(nsn->srcPath, file);
	serialize_string(nsn->name, file);
	
	serialize_int(nsn->count_methods, file);
	
	//write methods
	for(int i=0;i < nsn->count_methods;i++){ 
		struct Method* m = nsn->methods[i];
		writeMethod(m,file);
	}
	
	serialize_int(nsn->count_structs, file);
	
	//write structs
	for(int i=0;i < nsn->count_structs;i++){ 
		struct StructDecl* m = nsn->structs[i];
		writeStructDecl(m,file);
	}
	
	magic_num_serialize(MAGIC_END_NAMESPACE, file);
	
}
void writeMethod(struct Method* m, FILE* file){
	
	magic_num_serialize(MAGIC_METHOD, file);
	
	serialize_int(m->isPublic, file);
	serialize_int(m->hasSideEffects, file);
	
	serialize_string(m->name, file);
	
	writeType(m->returnType,file);

	serialize_int(m->count_args, file);
	
	for(int i = 0;i < m->count_args;i++){ 
		struct DeclArg* arg = m->args[i];
		writeDeclArg(arg, file); 
	}

	writeStmtBlock(m->block, file);
	
	magic_num_serialize(MAGIC_END_METHOD, file);
}
void writeStructDecl(struct StructDecl* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRUCTDECL, file);
	
	writeSimpleType(m->type, file);
	
	serialize_int(m->count_members, file);
	
	for(int i=0;i < m->count_members;i++){
		writeStructMember(m->members[i], file);
	}
	
	magic_num_serialize(MAGIC_END_STRUCTDECL, file);
}
void writeStructMember(struct StructMember* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRUCTMEMBER, file);
	
	writeType(m->type, file);
	
	serialize_string(m->name, file);
	
	magic_num_serialize(MAGIC_END_STRUCTMEMBER, file);
}

void writeStmtBlock(struct StmtBlock* block, FILE* file){
	
	magic_num_serialize(MAGIC_STMTBLOCK, file);
	
	serialize_int(block->count, file);
	
	for(int i=0;i < block->count; i++){
		writeStmt(block->stmts[i], file);
	}
	
	magic_num_serialize(MAGIC_END_STMTBLOCK, file);
}

// ----------------------------

void writeDeclArg(struct DeclArg* m, FILE* file){

	magic_num_serialize(MAGIC_DECLARG, file);

	writeType(m->type,file);
	
	int option = (m->name != NULL)?OPT_PRESENT:OPT_EMPTY;
	
	serialize_int(option, file);
	
	if(m->name != NULL){
		serialize_string(m->name, file);
	}
	
	magic_num_serialize(MAGIC_END_DECLARG, file);
}
void writeVariable(struct Variable* m, FILE* file){

	magic_num_serialize(MAGIC_VARIABLE, file);

	writeSimpleVar(m->simpleVar,file);

	int hasMemberAccess = (m->memberAccess == NULL)?OPT_EMPTY:OPT_PRESENT;

	serialize_int(hasMemberAccess, file);
	
	if(hasMemberAccess == OPT_PRESENT){

		writeVariable(m->memberAccess, file);
	}

	magic_num_serialize(MAGIC_END_VARIABLE, file);
}
void writeSimpleVar(struct SimpleVar* m, FILE* file){

	magic_num_serialize(MAGIC_SIMPLEVAR, file);

	serialize_string(m->name, file);

	serialize_int(m->count_indices, file);
	
	for(int i=0;i < m->count_indices; i++){
		writeExpr(m->indices[i], file);
	}
	
	magic_num_serialize(MAGIC_END_SIMPLEVAR, file);
}
void writeExpr(struct Expr* m, FILE* file){

	magic_num_serialize(MAGIC_EXPR, file);

	writeUnOpTerm(m->term1, file);
	
	if(m->op != NULL){
		serialize_int(OPT_PRESENT, file);
		
		writeOp(m->op,file);
		writeUnOpTerm(m->term2,file);
	}else{
		serialize_int(OPT_EMPTY, file);
	}
	
	magic_num_serialize(MAGIC_END_EXPR, file);
}
void writeTerm(struct Term* m, FILE* file){
	
	magic_num_serialize(MAGIC_TERM, file);

	serialize_int(m->kind, file);

	switch(m->kind){
		case  1: writeBoolConst(m->ptr.m1,file); break;
		case  2: writeIntConst(m->ptr.m2,file); break;
		case  3: writeCharConst(m->ptr.m3,file); break;
		case  4: writeMethodCall(m->ptr.m4,file); break;
		case  5: writeExpr(m->ptr.m5,file); break;
		case  6: writeVariable(m->ptr.m6,file); break;
		case  7: writeFloatConst(m->ptr.m7,file); break;
		case  8: writeStringConst(m->ptr.m8, file); break;
		case  9: writeHexConst(m->ptr.m9, file); break;
		case 10: writeBinConst(m->ptr.m10, file); break;
		default:
			printf("Error in writeTerm(...)\n");
			fclose(file);
			exit(1);
	}
	
	magic_num_serialize(MAGIC_END_TERM, file);
}
void writeUnOpTerm(struct UnOpTerm* t, FILE* file){
	
	magic_num_serialize(MAGIC_UNOPTERM, file);
	
	serialize_int((t->op == NULL)?OPT_EMPTY:OPT_PRESENT, file);
	
	if(t->op != NULL){
		writeOp(t->op, file);
	}
	
	writeTerm(t->term, file);
	
	magic_num_serialize(MAGIC_END_UNOPTERM, file);
}
void writeRange(struct Range* r, FILE* file){
	
	magic_num_serialize(MAGIC_RANGE, file);
	
	writeExpr(r->start, file);
	writeExpr(r->end, file);
	
	magic_num_serialize(MAGIC_END_RANGE, file);
}
// --------- CONST NODES ----------------
void writeBoolConst(struct BoolConst* m, FILE* file){
	magic_num_serialize(MAGIC_BOOLCONST, file);
	fwrite(m, sizeof(struct BoolConst), 1, file);
	magic_num_serialize(MAGIC_END_BOOLCONST, file);
}
void writeIntConst(struct IntConst* m, FILE* file){
	magic_num_serialize(MAGIC_INTCONST, file);
	fwrite(m, sizeof(struct IntConst), 1, file);
	magic_num_serialize(MAGIC_END_INTCONST, file);
}
void writeHexConst(struct HexConst* m, FILE* file){
	magic_num_serialize(MAGIC_HEXCONST, file);
	fwrite(m, sizeof(struct HexConst), 1, file);
	magic_num_serialize(MAGIC_END_HEXCONST, file);
}
void writeBinConst(struct BinConst* m, FILE* file){
	magic_num_serialize(MAGIC_BINCONST, file);
	fwrite(m, sizeof(struct BinConst), 1, file);
	magic_num_serialize(MAGIC_END_BINCONST, file);
}
void writeCharConst(struct CharConst* m, FILE* file){
	magic_num_serialize(MAGIC_CHARCONST, file);
	fwrite(m, sizeof(struct CharConst), 1, file);
	magic_num_serialize(MAGIC_END_CHARCONST, file);
}
void writeFloatConst(struct FloatConst* m, 	FILE* file){
	magic_num_serialize(MAGIC_FLOATCONST, file);
	fwrite(m, sizeof(struct FloatConst), 1, file);
	magic_num_serialize(MAGIC_END_FLOATCONST, file);
}
void writeStringConst(struct StringConst* m, FILE* file){
	
	magic_num_serialize(MAGIC_STRINGCONST, file);
	
	serialize_string(m->value, file);
	
	magic_num_serialize(MAGIC_END_STRINGCONST, file);
}
void writeOp(struct Op* m, FILE* file){
	
	magic_num_serialize(MAGIC_OP, file);
	
	fwrite(m, sizeof(struct Op), 1, file);
	
	magic_num_serialize(MAGIC_END_OP, file);
}
// ---------------- STATEMENTNODES ---------------------
void writeStmt(struct Stmt* m, FILE* file){

	magic_num_serialize(MAGIC_STMT, file);

	//the reader has to know which type it is,
	//we can print a small number
	serialize_int(m->kind, file);
	
	switch(m->kind){
		case 99: 
			{
				serialize_int( m->isBreak ? OPT_PRESENT : OPT_EMPTY, file);
				serialize_int( m->isContinue ? OPT_PRESENT : OPT_EMPTY, file);
			}
			break;
		case 0: { writeLoopStmt(m->ptr.m0, file);  } break;
		case 1: { writeMethodCall(m->ptr.m1,file); } break;
		case 2: { writeWhileStmt(m->ptr.m2,file);  } break;
		case 3: { writeIfStmt(m->ptr.m3,file);     } break;
		case 4: { writeRetStmt(m->ptr.m4,file);    } break;
		case 5: { writeAssignStmt(m->ptr.m5,file); } break;
		case 7: { writeForStmt(m->ptr.m7,file);    } break;
		case 8: { writeSwitchStmt(m->ptr.m8,file); } break;
		default: 
			printf("Error in writeStmt\n");
			fclose(file);
			exit(1);
			break;
	}
	
	magic_num_serialize(MAGIC_END_STMT, file);
}

void writeIfStmt(struct IfStmt* m, FILE* file){

	magic_num_serialize(MAGIC_IFSTMT, file);

	writeExpr(m->condition, file);

	writeStmtBlock(m->block, file);
	
	//indicate if there is an else block
	serialize_int((m->elseBlock != NULL)?OPT_PRESENT:OPT_EMPTY, file);
	
	if(m->elseBlock != NULL){
		writeStmtBlock(m->elseBlock, file);
	}
	
	magic_num_serialize(MAGIC_END_IFSTMT, file);
}

void writeWhileStmt(struct WhileStmt* m, FILE* file){

	magic_num_serialize(MAGIC_WHILESTMT, file);

	writeExpr(m->condition, file);

	writeStmtBlock(m->block, file);
	
	magic_num_serialize(MAGIC_END_WHILESTMT, file);
}

void writeRetStmt(struct RetStmt* m, FILE* file){

	magic_num_serialize(MAGIC_RETSTMT, file);

	writeExpr(m->returnValue,file);
	
	magic_num_serialize(MAGIC_END_RETSTMT, file);
}

void writeAssignStmt(struct AssignStmt* m, FILE* file){

	magic_num_serialize(MAGIC_ASSIGNSTMT, file);

	const int option = (m->optType != NULL)?OPT_PRESENT:OPT_EMPTY;
	serialize_int(option, file);

	if(m->optType != NULL){
		writeType(m->optType, file);
	}
	
	writeVariable(m->var, file);
	
	serialize_string(m->assign_op, file);
	
	writeExpr(m->expr, file);
	
	magic_num_serialize(MAGIC_END_ASSIGNSTMT, file);
}
void writeMethodCall(struct MethodCall* m, FILE* file){

	magic_num_serialize(MAGIC_METHODCALL, file);

	serialize_string(m->methodName, file);
	
	serialize_int(m->count_args, file);
	
	for(int i=0;i < m->count_args;i++){ 
		struct Expr* e = m->args[i];
		writeExpr(e,file); 
	}
	
	magic_num_serialize(MAGIC_END_METHODCALL, file);
}
void writeLoopStmt(struct LoopStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_LOOPSTMT, file);
	
	writeExpr(m->count, file);

	writeStmtBlock(m->block, file);
	
	magic_num_serialize(MAGIC_END_LOOPSTMT, file);
}
void writeForStmt(struct ForStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_FORSTMT, file);
	
	serialize_string(m->indexName, file);

	writeRange(m->range, file);

	writeStmtBlock(m->block, file);
	
	magic_num_serialize(MAGIC_END_FORSTMT, file);
}
void writeSwitchStmt(struct SwitchStmt* m,	FILE* file){
	
	magic_num_serialize(MAGIC_SWITCHSTMT, file);
	
	writeVariable(m->var, file);
	
	serialize_int(m->count_cases, file);
	
	for(int i=0; i < m->count_cases; i++){
		
		writeCaseStmt(m->cases[i], file);
	}
	
	magic_num_serialize(MAGIC_END_SWITCHSTMT, file);
}
void writeCaseStmt(struct CaseStmt* m, FILE* file){
	
	magic_num_serialize(MAGIC_CASESTMT, file);
	
	serialize_int(m->kind, file);
	
	switch(m->kind){
		case 0: writeBoolConst(m->ptr.m1, file); break;
		case 1: writeCharConst(m->ptr.m2, file); break;
		case 2: writeIntConst(m->ptr.m3, file); break;
		default: 
			printf("Error\n");
			fclose(file);
			exit(1);
	}
	
	serialize_int((m->block == NULL)?0:1, file);
	
	if(m->block != NULL){
		
		writeStmtBlock(m->block, file);
	}
	
	magic_num_serialize(MAGIC_END_CASESTMT, file);
}
// --------- TYPENODES --------------
void writeType(struct Type* m, FILE* file){
	//there is an alternative. we give a small number 
	//to indicate the alternative
	
	magic_num_serialize(MAGIC_TYPE, file);
	
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
	
	writeType(m->element_type,file);
	
	magic_num_serialize(MAGIC_END_ARRAYTYPE, file);
}

void writeTypeParam(struct TypeParam* m, FILE* file){
	
	magic_num_serialize(MAGIC_TYPEPARAM, file);
	
	fwrite(m, sizeof(struct TypeParam), 1, file);
	
	magic_num_serialize(MAGIC_END_TYPEPARAM, file);
}

void writeBasicTypeWrapped(struct BasicTypeWrapped* m, FILE* file){

	magic_num_serialize(MAGIC_BASICTYPEWRAPPED, file);

	if(m->simpleType != NULL){ 
		
		serialize_int(1, file);
		writeSimpleType(m->simpleType,file); 
		
	}else if(m->subrType != NULL){ 
		
		serialize_int(2, file);
		writeSubrType(m->subrType,file); 
		
	}
	
	magic_num_serialize(MAGIC_END_BASICTYPEWRAPPED, file);
}

void writeSimpleType(struct SimpleType* m, FILE* file){
	
	magic_num_serialize(MAGIC_SIMPLETYPE, file);
	
	serialize_string(m->typeName, file);
	
	serialize_int(m->isPrimitive, file);
	
	serialize_int(m->isIntType, file);
	serialize_int(m->isFloatType, file);
	serialize_int(m->isCharType, file);
	serialize_int(m->isBoolType, file);
	
	serialize_int(m->typeParamCount, file);
	for(int i=0;i<m->typeParamCount;i++){
		serialize_int(m->typeParams[i], file);
	}
	
	magic_num_serialize(MAGIC_END_SIMPLETYPE, file);
}

void writeSubrType(struct SubrType* m, FILE* file){

	magic_num_serialize(MAGIC_SUBRTYPE, file);

	writeType(m->returnType,file);
	serialize_int(m->hasSideEffects, file);

	serialize_int(m->count_argTypes, file);
	for(int i = 0;i < m->count_argTypes;i++){
		struct Type* t = m->argTypes[i];
		writeType(t,file); 
	}
	
	magic_num_serialize(MAGIC_END_SUBRTYPE, file);
}
// --------- OTHER ----------
void write_ast(char* filename, struct Namespace* namespaceNode){
	
	FILE* file = fopen(filename, "w");

	if(file == NULL){
		printf("could not open file: %s (in write_ast)\n", filename);
		exit(1);
	}
	
	magic_num_serialize(MAGIC_AST, file);
	
	writeNamespace(namespaceNode, file);
	
	magic_num_serialize(MAGIC_END_AST, file);
	
	fclose(file);
}
