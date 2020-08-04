#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast_reader.h"
#include "../util/util.h"

struct AST_Whole_Program* readAST(char* filename, bool debug){
	
	if(debug){
			printf("readAST(...)\n");
	}
	
	FILE* file = fopen(filename,"r");
	if(file == NULL){return NULL;}

	struct AST_Whole_Program* ast = smalloc(sizeof(struct AST_Whole_Program));

	struct Namespace* ns = readNamespace(file, debug);
	if(ns == NULL){return NULL;}
	ast->count_namespaces = 1;

	ast->namespaces = smalloc(sizeof(struct Namespace*));
	ast->namespaces[0] = ns;

	fclose(file);
	return ast;
}

struct Namespace* readNamespace(FILE* file, bool debug){
	if(debug){
			printf("readNamespace(...)\n");
	}
	struct Namespace* ns = smalloc(sizeof(struct Namespace));

	int count = fscanf(file,
		"%s\t%s\t%d\t", 
		ns->srcPath, 
		ns->name, 
		&(ns->count_methods)
	);

	if(count != 3){
		fclose(file);
		printf("error in readNamespace ,%d\n",count);
		exit(1);
	}

	//read methods
	ns->methods = smalloc(sizeof(struct Method*)*(ns->count_methods));
	if(debug){
		printf("reading %d Methods\n", ns->count_methods);
	}
	for(int i=0; i < ns->count_methods; i++){
		struct Method* m =  readMethod(file, debug);
		if(m == NULL){
			fclose(file);
			printf("error in readNamespace 2\n");
			exit(1);
		}
		ns->methods[i] = m;
	}
	
	//read structs
	count = fscanf(file, "%d\t", &(ns->count_structs));
	if(debug){
		printf("reading %d Structs\n", ns->count_structs);
	}
	if(count != 1){
		fclose(file);
		printf("error in readNamespace 3 ,%d\n",count);
		exit(1);
	}
	ns->structs = smalloc(sizeof(struct StructDecl*)*(ns->count_structs));
	for(int i=0;i < ns->count_structs; i++){
		struct StructDecl* s = readStructDecl(file, debug);
		if(s == NULL){
			fclose(file);
			printf("error in readNamespace 4\n");
			exit(1);
		}
		ns->structs[i] = s;
	}
	
	return ns;
}
struct Method* readMethod(FILE* file, bool debug){
	if(debug){
			printf("readMethod(...)\n");
	}
	if(fscanf(file, "Method\t") == EOF){
		printf("Error reading Method\n");
		exit(1);
	}
	struct Method* m = smalloc(sizeof(struct Method));

	fscanf(file,"%d\t%d\t%s\t",(int*)&(m->isPublic),(int*)&(m->hasSideEffects), m->name);

	m->returnType = readType(file, debug);
	if(m->returnType == NULL){return NULL;}

	fscanf(file,"%d\t",&(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = readDeclArg(file, debug);
	}

	m->block = readStmtBlock(file, debug);
	
	return m;
}
struct StructDecl* readStructDecl(FILE* file, bool debug){
	
	if(debug){
			printf("readStructDecl(...)\n");
	}
	
	struct StructDecl* res = smalloc(sizeof(struct StructDecl));
	
	if(fscanf(file, "StructDecl\t") == EOF){
		printf("Error reading StructDecl\n");
		char buffer[14];
		
		fread(buffer, sizeof(char), 14, file);
		buffer[13] = '\0';
		printf("actual: %s\n", buffer);
		exit(1);
	}
	
	fscanf(file, "%s\t%d\t",res->name, &(res->count_members));
	
	res->members = malloc(sizeof(struct StructMember*)*res->count_members);
	for(int i=0;i < res->count_members;i++){
		res->members[i] = readStructMember(file, debug);
	}
	return res;
}
struct StructMember* readStructMember(FILE* file, bool debug){
	if(debug){
			printf("readStructMember(...)\n");
	}
	
	struct StructMember* res = smalloc(sizeof(struct StructMember));
	if(fscanf(file, "StructMember\t") == EOF){
		printf("Error reading StructMember\n");
		exit(1);
	}
	res->type = readType(file, debug);
	fscanf(file, "%s\t", res->name);
	return res;
}

struct StmtBlock* readStmtBlock(FILE* file, bool debug){
	if(debug){
			printf("readStmtBlock(...)\n");
	}
	struct StmtBlock* block = smalloc(sizeof(struct StmtBlock));
	
	if(fscanf(file, "StmtBlock\t") == EOF){
		printf("Error reading StmtBlock\n");
		exit(1);
	}
	
	fscanf(file, "%d\t", &(block->count));
	block->stmts = smalloc(sizeof(struct Stmt*)* block->count);
	for(int i=0;i < block->count; i++){
		block->stmts[i] = readStmt(file, debug);
	}
	return block;
}

// -----------------------

struct DeclArg* readDeclArg(FILE* file, bool debug){
	if(debug){
			printf("readDeclArg(...)\n");
	}
	struct DeclArg* da = smalloc(sizeof(struct DeclArg));

	if(fscanf(file, "DeclaredArg\t") == EOF){
		printf("Error reading DeclaredArg\n");
		exit(1);
	}

	da->type = readType(file, debug);

	char next[10];
	fscanf(file, "%s\t", next);
	if(strcmp(next, "NULL")==0){
		da->has_name = false;
	}else{
		strcpy(da->name, next);
		da->has_name = true;
	}

	return da;
}
struct Expr* readExpr(FILE* file, bool debug){
	
	if(debug){
			printf("readExpr(...)\n");
	}
	
	struct Expr* expr = smalloc(sizeof(struct Expr));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Expr") != 0){ 
		printf("Error reading Expr\n");
		exit(1);
	}

	expr->term1 = readTerm(file, debug);
	fscanf(file, "%s\t",next);
	if(strcmp(next, "NULL")==0){
		expr->op = NULL;
		expr->term2 = NULL;
	}else{
		expr->op = readOp(file, debug);
		expr->term2 = readTerm(file, debug);
	}

	return expr;
}
struct Op* readOp(FILE* file, bool debug){
	
	if(debug){
			printf("readOp(...)\n");
	}
	
	struct Op* op = smalloc(sizeof(struct Op));
	char next[10];
	fscanf(file, "%s\t",next);
	if(strcmp(next,"Op") != 0){ 
		printf("Error reading Op\n");
		exit(1);
	}

	fscanf(file, "%s\t", op->op);
	return op;
}
struct IntConst* readIntConst(FILE* file, bool debug){
	
	if(debug){
			printf("readIntConst(...)\n");
	}
	
	struct IntConst* ic = smalloc(sizeof(struct IntConst));
	
	if(fscanf(file, "IntConst\t") == EOF){
		printf("Error reading IntConst\n");
		exit(1);
	}

	fscanf(file, "%d\t", &(ic->value));
	return ic;
}
struct BoolConst* readBoolConst(FILE* file, bool debug){
	
	if(debug){
			printf("readBoolConst(...)\n");
	}
	
	struct BoolConst* b = smalloc(sizeof(struct BoolConst));
	
	if(fscanf(file, "BoolConst\t") == EOF){
		printf("Error reading BoolConst\n");
		exit(1);
	}

	fscanf(file, "%d\t", (int*)(b->value));
	return b;
}
struct CharConst* readCharConst(FILE* file, bool debug){
	
	if(debug){
			printf("readCharConst(...)\n");
	}
	
	struct CharConst* b = smalloc(sizeof(struct CharConst));
	
	if(fscanf(file, "CharConst\t") == EOF){
		printf("Error reading CharConst\n");
		exit(1);
	}

	fscanf(file, "%c\t", &(b->value));
	return b;
}
struct FloatConst* readFloatConst(FILE* file, bool debug){
	
	if(debug){
			printf("readFloatConst(...)\n");
	}
	
	struct FloatConst* ic = smalloc(sizeof(struct FloatConst));
	
	if(fscanf(file, "FloatConst\t") == EOF){
		printf("Error reading FloatConst\n");
		exit(1);
	}

	fscanf(file, "%f\t", &(ic->value));
	return ic;
}
struct Variable* readVariable(FILE* file, bool debug){
	
	if(debug){
			printf("readVariable(...)\n");
	}
	
	struct Variable* v = smalloc(sizeof(struct Variable));
	
	if(fscanf(file, "Variable\t") == EOF){
		printf("Error reading Variable\n");
		exit(1);
	}

	v->simpleVar = readSimpleVar(file, debug);
	int memberAccessCount = 0;
	fscanf(file, "%d\t", &memberAccessCount);

	v->memberAccessList = smalloc(sizeof(struct SimpleVar*)*memberAccessCount);
	for(int i=0;i<memberAccessCount;i++){
		v->memberAccessList[i] = readVariable(file, debug);
	}
	return v;
}
struct SimpleVar* readSimpleVar(FILE* file, bool debug){
	
	if(debug){
			printf("readSimpleVar(...)\n");
	}
	
	struct SimpleVar* b = smalloc(sizeof(struct SimpleVar));
	char next[10];
	
	if(fscanf(file, "SimpleVar\t") == EOF){
		printf("Error reading SimpleVar\n");
		exit(1);
	}

	fscanf(file, "%s\t", b->name);
	fscanf(file, "%s\t",next);

	if(strcmp(next, "NULL") != 0){
		b->optIndex = readExpr(file, debug);
	}else{
		b->optIndex = NULL;
	}
	return b;
}
struct Term* readTerm(FILE* file, bool debug){
	
	if(debug){
			printf("readTerm(...)\n");
	}
	
	struct Term* b = smalloc(sizeof(struct Term));
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;
	b->m4 = NULL;
	b->m5 = NULL;
	b->m6 = NULL;
	b->m7 = NULL;

	char next[10];
	
	if(fscanf(file, "Term\t") == EOF){
		printf("Error reading Term\n");
		exit(1);
	}
	
	int kind;
	fscanf(file, "%d\t", &kind);
	kind = atoi(next);

	switch(kind){
		case 1: b->m1 = readBoolConst(file, debug); break;
		case 2: b->m2 = readIntConst(file, debug); break;
		case 3: b->m3 = readCharConst(file, debug); break;
		case 4: b->m4 = readMethodCall(file, debug); break;
		case 5: b->m5 = readExpr(file, debug); break;
		case 6: b->m6 = readVariable(file, debug); break;
		case 7: b->m7 = readFloatConst(file, debug); break;
	}
	return b;
}

//statementnodes
struct Stmt* readStmt(FILE* file, bool debug){
	if(debug){
			printf("readStmt(...)\n");
	}
	struct Stmt* b = smalloc(sizeof(struct Stmt));
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;
	b->m4 = NULL;
	b->m5 = NULL;

	char next[10];
	
	if(fscanf(file, "Stmt\t") == EOF){
		printf("Error reading Stmt\n");
		exit(1);
	}
	
	int kind;
	fscanf(file, "%d\t", &kind);
	kind = atoi(next);

	switch(kind){
		case 1: b->m1 = readMethodCall(file, debug); break;
		case 2: b->m2 = readWhileStmt(file, debug); break;
		case 3: b->m3 = readIfStmt(file, debug); break;
		case 4: b->m4 = readRetStmt(file, debug); break;
		case 5: b->m5 = readAssignStmt(file, debug); break;
	}
	return b;
}
struct IfStmt* readIfStmt(FILE* file, bool debug){
	
	if(debug){
			printf("readIfStmt(...)\n");
	}
	
	struct IfStmt* v = smalloc(sizeof(struct IfStmt));
	
	v->condition = NULL;
	v->block = NULL;
	v->elseBlock = NULL;
	
	if(fscanf(file, "IfStmt\t") == EOF){
		printf("Error reading IfStmt\n");
		exit(1);
	}

	v->condition = readExpr(file, debug);

	v->block = readStmtBlock(file, debug);

	int hasElse=0;
	fscanf(file, "%d\t", &hasElse);
	if(hasElse != 0){
		v->elseBlock = readStmtBlock(file, debug);
	}
	return v;
}
struct WhileStmt* readWhileStmt(FILE* file, bool debug){
	if(debug){
			printf("readWhileStmt(...)\n");
	}
	struct WhileStmt* v = smalloc(sizeof(struct WhileStmt));
	
	if(fscanf(file, "WhileStmt\t") == EOF){
		printf("Error reading WhileStmt\n");
		exit(1);
	}

	v->condition = readExpr(file, debug);
	v->block = readStmtBlock(file, debug);
	
	return v;
}
struct RetStmt* readRetStmt(FILE* file, bool debug){
	if(debug){
			printf("readRetStmt(...)\n");
	}
	struct RetStmt* v = smalloc(sizeof(struct RetStmt));
	
	if(fscanf(file, "RetStmt\t") == EOF){
		printf("Error reading RetStmt\n");
		exit(1);
	}

	v->returnValue = readExpr(file, debug);
	return v;
}
struct AssignStmt* readAssignStmt(FILE* file, bool debug){
	if(debug){
			printf("readAssignStmt(...)\n");
	}
	struct AssignStmt* v = smalloc(sizeof(struct AssignStmt));
	char next[10];
	
	if(fscanf(file, "AssignStmt\t") == EOF){
		printf("Error reading AssignStmt\n");
		exit(1);
	}

	fscanf(file, "%s\t", next);
	if(strcmp(next, "NULL") == 0){
		v->optType = NULL;
	}else{
		v->optType = readType(file, debug);
	}

	v->var = readVariable(file, debug);
	v->expr = readExpr(file, debug);

	return v;
}
struct MethodCall* readMethodCall(FILE* file, bool debug){
	if(debug){
			printf("readMethodCall(...)\n");
	}
	struct MethodCall* v = smalloc(sizeof(struct MethodCall));
	
	if(fscanf(file, "MethodCall\t") == EOF){
		printf("Error reading MethodCall\n");
		exit(1);
	}

	fscanf(file, "%s\t%d\t", v->methodName, &(v->count_args));

	v->args = smalloc(sizeof(struct Expr*)*(v->count_args));
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = readExpr(file, debug);
	}
	return v;
}

//typenodes
struct Type* readType(FILE* file, bool debug){
	if(debug){
			printf("readType(...)\n");
	}
	struct Type* b = smalloc(sizeof(struct Type));
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;
	
	if(fscanf(file, "Type\t") == EOF){
		printf("Error reading Type\n");
		exit(1);
	}
	
	int kind;
	fscanf(file, "%d\t", &kind);

	switch(kind){
		case 1: b->m1 = readBasicTypeWrapped(file, debug); break;
		case 2: b->m2 = readTypeParam(file, debug); break;
		case 3: b->m3 = readArrayType(file, debug); break;
	}
	return b;
}
struct SubrType* readSubrType(FILE* file, bool debug){
	if(debug){
			printf("readSubrType(...)\n");
	}
	struct SubrType* v = smalloc(sizeof(struct SubrType));
	
	if(fscanf(file, "SubrType\t") == EOF){
		printf("Error reading SubrType\n");
		exit(1);
	}

	v->returnType = readType(file, debug);
	fscanf(file, "%d\t", (int*)(v->hasSideEffects));

	fscanf(file, "%d\t", &(v->count_argTypes));
	v->argTypes = smalloc(sizeof(struct Type*)*(v->count_argTypes));
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file, debug);
	}
	return v;
}
struct SimpleType* readSimpleType(FILE* file, bool debug){
	if(debug){
			printf("readSimpleType(...)\n");
	}
	struct SimpleType* v = smalloc(sizeof(struct SimpleType));
	
	if(fscanf(file, "SimpleType\t") == EOF){
		printf("Error reading SimpleType\n");
		exit(1);
	}
	
	fscanf(file, "%s\t", v->typeName);
	return v;
}
struct ArrayType* readArrayType(FILE* file, bool debug){
	if(debug){
			printf("readArrayType(...)\n");
	}
	struct ArrayType* v = smalloc(sizeof(struct ArrayType));
	
	if(fscanf(file, "ArrayType\t") == EOF){
	
		printf("Error reading ArrayType\n");
		exit(1);
	}
	v->element_type = readType(file, debug);
	return v;
}
struct TypeParam* readTypeParam(FILE* file, bool debug){
	if(debug){
			printf("readTypeParam(...)\n");
	}
	struct TypeParam* v = smalloc(sizeof(struct TypeParam));
	
	if(fscanf(file, "TypeParam\t") == EOF){
		printf("Error reading TypeParam\n");
		exit(1);
	}
	fscanf(file, "%d\t", &(v->index));
	return v;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug){
	if(debug){
			printf("readBasicTypeWrapped(...)\n");
	}
	
	struct BasicTypeWrapped* v = smalloc(sizeof(struct BasicTypeWrapped));
	
	if(fscanf(file, "BasicTypeWrapped\t") == EOF){
		printf("Error reading BasicTypeWrapped\n");
		exit(1);
	}
	
	int kind = 0;
	fscanf(file, "%d\t", &kind);
	switch(kind){
		case 1: 
			v->simpleType = readSimpleType(file, debug);
			v->subrType = NULL;
			break;
		case 2: 
			v->simpleType = NULL;
			v->subrType = readSubrType(file, debug);
			break;
	}
	return v;
}

