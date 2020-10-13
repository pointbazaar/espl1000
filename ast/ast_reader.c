#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast_reader.h"

// ---- these subroutines can be private, because users
// ---- of this module are probably only ever going to want
// ---- to read a full '.ast' file

struct Namespace* readNamespace(FILE* file, bool debug);
struct Method* readMethod(FILE* file, bool debug);
struct StructDecl* readStructDecl(FILE* file, bool debug);
struct StructMember* readStructMember(FILE* file, bool debug);
struct StmtBlock* readStmtBlock(FILE* file, bool debug);

struct DeclArg* readDeclArg(FILE* file, bool debug);
struct Expr* readExpr(FILE* file, bool debug);
struct Op* readOp(FILE* file, bool debug);

//const nodes
struct IntConst* 	readIntConst(	FILE* file, bool debug);
struct BoolConst* 	readBoolConst(	FILE* file, bool debug);
struct CharConst* 	readCharConst(	FILE* file, bool debug);
struct FloatConst* 	readFloatConst(	FILE* file, bool debug);
struct StringConst* readStringConst(FILE* file, bool debug);

struct Variable* readVariable(FILE* file, bool debug);
struct SimpleVar* readSimpleVar(FILE* file, bool debug);
struct Term* readTerm(FILE* file, bool debug);
struct UnOpTerm* readUnOpTerm(FILE* file, bool debug);
struct Range* readRange(FILE* file, bool debug);

//statementnodes
struct Stmt* readStmt(FILE* file, bool debug);
struct IfStmt* readIfStmt(FILE* file, bool debug);
struct WhileStmt* readWhileStmt(FILE* file, bool debug);
struct RetStmt* readRetStmt(FILE* file, bool debug);
struct AssignStmt* readAssignStmt(FILE* file, bool debug);
struct MethodCall* readMethodCall(FILE* file, bool debug);
struct LoopStmt* readLoopStmt(FILE* file, bool debug);
struct BreakStmt* readBreakStmt(FILE* file, bool debug);
struct ForStmt* readForStmt(FILE* file, bool debug);
struct SwitchStmt* readSwitchStmt(FILE* file, bool debug);
struct CaseStmt* readCaseStmt(FILE* file, bool debug);

//typenodes
struct Type* readType(FILE* file, bool debug);
struct SubrType* readSubrType(FILE* file, bool debug);
struct SimpleType* readSimpleType(FILE* file, bool debug);
struct ArrayType* readArrayType(FILE* file, bool debug);
struct TypeParam* readTypeParam(FILE* file, bool debug);
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug);


// ----

struct AST_Whole_Program* readAST(char* filename, bool debug){
	//returns NULL if it is unable to open the file
	
	if(debug){ printf("readAST(...)\n"); }
	
	FILE* file = fopen(filename,"r");
	if(file == NULL){
		printf("could not open file: %s, in ast_reader.c:readAST\n", filename);
		return NULL;
	}

	struct AST_Whole_Program* ast = malloc(sizeof(struct AST_Whole_Program));

	struct Namespace* ns = readNamespace(file, debug);
	
	ast->count_namespaces = 1;
	ast->namespaces = 
		malloc(sizeof(struct Namespace*));
	ast->namespaces[0] = ns;

	fclose(file);
	
	if(debug){ printf("readAST ~ done\n"); }
	
	return ast;
}

struct Namespace* readNamespace(FILE* file, bool debug){
	
	if(debug){ printf("readNamespace(...)\n"); }
	
	struct Namespace* ns = malloc(sizeof(struct Namespace));

	int count = fscanf(file,
		"%s\t%s\t%u\t", 
		ns->srcPath, 
		ns->name, 
		(unsigned int*)(&(ns->count_methods))
	);

	if(count != 3){
		fclose(file);
		printf("error in readNamespace ,%d\n",count);
		free(ns);
		exit(1);
	}

	//read methods
	ns->methods = 
		malloc(sizeof(struct Method*)*(ns->count_methods));
	if(debug){
		printf("reading %d Methods\n", ns->count_methods);
	}
	for(int i=0; i < ns->count_methods; i++){
		
		struct Method* m =  readMethod(file, debug);
		ns->methods[i] = m;
	}
	
	//read structs
	int count_structs = 0;
	count = fscanf(file, "%d\t", &count_structs);
	
	if(count != 1){
		fclose(file);
		printf("error in readNamespace 3 ,%d\n",count);
		free(ns);
		exit(1);
	}
	
	ns->count_structs = count_structs;
	
	if(debug){
		printf("reading %hd Structs\n", ns->count_structs);
	}
	
	ns->structs = 
		malloc(sizeof(struct StructDecl*)*(ns->count_structs));
	for(int i=0;i < ns->count_structs; i++){
		
		ns->structs[i] = readStructDecl(file, debug);
	}
	
	if(debug){ printf("done\n"); }
	
	return ns;
}
struct Method* readMethod(FILE* file, bool debug){
	
	if(debug){ printf("readMethod(...)\n"); }
	
	struct Method* m = malloc(sizeof(struct Method));

	if(fscanf(
		file,
		"Method\t%d\t%d\t%s\t",
		(int*)&(m->isPublic),
		(int*)&(m->hasSideEffects), 
		m->name) != 3
	){
		printf("Error reading Method \n");
		fclose(file);
		free(m);
		exit(1);
	}

	m->returnType = readType(file, debug);

	if(fscanf(file,"%u\t",(unsigned int*)&(m->count_args)) != 1){
		printf("Error reading Method 2\n");
		fclose(file);
		free(m);
		exit(1);
	}
	
	m->args = 
		malloc(sizeof(struct DeclArg*)*(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = readDeclArg(file, debug);
	}

	m->block = readStmtBlock(file, debug);
	
	if(debug){ printf("done\n"); }
	
	return m;
}
struct StructDecl* readStructDecl(FILE* file, bool debug){
	
	if(debug){ printf("readStructDecl(...)\n"); }
	
	struct StructDecl* res = 
		malloc(sizeof(struct StructDecl));
	
	if(fscanf(file, "StructDecl\t") == EOF){
		printf("Error reading StructDecl\n");
		fclose(file);
		free(res);
		exit(1);
	}
	
	res->type = readSimpleType(file, debug);
	
	int count_members = 0;
	
	if(
		fscanf(
			file, "%d\t", 
			&(count_members)
		) != 1
	){
		printf("Error reading StructDecl\n");
		fclose(file);
		free(res);
		exit(1);
	}
	
	res->count_members = count_members;
	
	res->members = 
		malloc(sizeof(struct StructMember*)*res->count_members);
	for(int i=0;i < res->count_members;i++){
		res->members[i] = readStructMember(file, debug);
	}
	
	if(debug){ printf("done\n"); }
	
	return res;
}
struct StructMember* readStructMember(FILE* file, bool debug){
	
	if(debug){ printf("readStructMember(...)\n"); }
	
	struct StructMember* res = 
		malloc(sizeof(struct StructMember));
	
	if(fscanf(file, "StructMember\t") == EOF){
		printf("Error reading StructMember\n");
		fclose(file);
		free(res);
		exit(1);
	}
	
	res->type = readType(file, debug);
	
	if(fscanf(file, "%s\t", res->name) != 1){
		printf("Error reading StructMember2\n");
		//freeType(res->type) 
		//but this subroutine is not included here
		fclose(file);
		free(res);
		exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
	return res;
}

struct StmtBlock* readStmtBlock(FILE* file, bool debug){
	
	if(debug){ printf("readStmtBlock(...)\n"); }
	
	struct StmtBlock* block = 
		malloc(sizeof(struct StmtBlock));
	
	if(fscanf(file, "StmtBlock\t%u\t", (unsigned int*)&(block->count)) != 1){
		printf("Error reading StmtBlock\n");
		fclose(file);
		free(block);
		exit(1);
	}
	
	block->stmts = 
		malloc(sizeof(struct Stmt*)* block->count);
	
	for(int i=0;i < block->count; i++){
		block->stmts[i] = readStmt(file, debug);
	}
	
	return block;
}

// -----------------------

struct DeclArg* readDeclArg(FILE* file, bool debug){
	
	if(debug){ printf("readDeclArg(...)\n"); }
	
	struct DeclArg* da = 
		malloc(sizeof(struct DeclArg));

	if(fscanf(file, "DeclaredArg\t") == EOF){
		printf("Error reading DeclaredArg\n");
		fclose(file);
		free(da);
		exit(1);
	}

	da->type = readType(file, debug);

	int option;
	if(fscanf(file, "%d\t", &option) != 1){
		printf("Error reading DeclaredArg 2\n");
		fclose(file);
		free(da);
		exit(1);
	}
	
	if(option == 0){
		da->has_name = false;
	}else if(option == 1){
		da->has_name = true;
		
		//do not read more than 19 chars
		if(fscanf(file, "%19s\t", da->name) != 1){
			printf("Error reading DeclaredArg 3\n");
			fclose(file);
			exit(1);
		}
	}else{
		printf("Error in readDeclArg\n");
		free(da);
		fclose(file);
		exit(1);
	}
	
	if(debug){ printf("done\n"); }

	return da;
}
struct Expr* readExpr(FILE* file, bool debug){
	
	if(debug){ printf("readExpr(...)\n"); }
	
	if(fscanf(file,"Expr\t") == EOF){ 
		printf("Error reading Expr\n");
		fclose(file);
		exit(1);
	}
	
	struct Expr* expr = 
		malloc(sizeof(struct Expr));

	expr->term1 = readUnOpTerm(file, debug);
	
	int option = 0;
	if(fscanf(file, "%d\t", &option) != 1){
		printf("Error reading Expr 2\n");
		fclose(file);
		exit(1);
	}
	
	if(option == 1){
		expr->op = NULL;
		expr->term2 = NULL;
	}else if (option == 0){
		expr->op = readOp(file, debug);
		expr->term2 = readUnOpTerm(file, debug);
	}

	return expr;
}
struct Op* readOp(FILE* file, bool debug){
	
	if(debug){ printf("readOp(...)\n"); }
	
	struct Op* op = 
		malloc(sizeof(struct Op));
	
	if(fscanf(file, "Op\t%s\t", op->op) != 1){ 
		printf("Error reading Op\n");
		free(op);
		fclose(file);
		exit(1);
	}

	return op;
}
struct IntConst* readIntConst(FILE* file, bool debug){
	
	if(debug){printf("readIntConst(...)\n");}
	
	struct IntConst* ic = 
		malloc(sizeof(struct IntConst));
	
	if(fscanf(file, "IntConst\t%d\t", &(ic->value)) != 1){
		printf("Error reading IntConst\n");
		free(ic);
		fclose(file);
		exit(1);
	}

	return ic;
}
struct BoolConst* readBoolConst(FILE* file, bool debug){
	
	if(debug){ printf("readBoolConst(...)\n"); }
	
	struct BoolConst* b = 
		malloc(sizeof(struct BoolConst));
	
	int val;
	if(fscanf(file, "BoolConst\t%d\t", &val) != 1){
		printf("Error reading BoolConst\n");
		free(b);
		fclose(file);
		exit(1);
	}
	
	b->value = val==1;
	
	return b;
}
struct CharConst* readCharConst(FILE* file, bool debug){
	
	if(debug){ printf("readCharConst(...)\n"); }
	
	struct CharConst* b = 
		malloc(sizeof(struct CharConst));
	
	if(fscanf(file, "CharConst\t%c\t", &(b->value)) != 1){
		printf("Error reading CharConst\n");
		free(b);
		fclose(file);
		exit(1);
	}
	
	return b;
}
struct FloatConst* readFloatConst(FILE* file, bool debug){
	
	if(debug){ printf("readFloatConst(...)\n"); }
	
	struct FloatConst* ic = 
		malloc(sizeof(struct FloatConst));
	
	if(fscanf(file, "FloatConst\t%f\t", &(ic->value)) != 1){
		printf("Error reading FloatConst\n");
		free(ic);
		fclose(file);
		exit(1);
	}
	
	return ic;
}
struct StringConst* readStringConst(FILE* file, bool debug){
	
	if(debug){ printf("readStringConst(...)\n"); }
	
	int length;
	if(fscanf(file, "StringConst\t%d\t", &length) != 1){
		printf("Error reading StringConst\n");
		fclose(file);
		exit(1);
	}
	
	struct StringConst* s = 
		malloc(sizeof(struct StringConst));
	
	//doing this to avoid problems
	//with whitespace or any characters at all really
	char* val = malloc(sizeof(char)*(length+1));
	val[length]='\0';
	
	for(int i=0;i < length; i++){
		//0-padded on the left, 3 chars wide,
		//casted to unsigned integer
		int v;
		fscanf(file, "%03d_", &v);
		val[i]=v;
	}
	s->value = val;
	
	return s;
}
struct Variable* readVariable(FILE* file, bool debug){
	
	if(debug){ printf("readVariable(...)\n"); }
	
	if(fscanf(file, "Variable\t") == EOF){
		printf("Error reading Variable\n");
		fclose(file);
		exit(1);
	}
	
	struct Variable* v = 
		malloc(sizeof(struct Variable));

	v->simpleVar = readSimpleVar(file, debug);
	
	int count = 0;
	if(fscanf(file, "%d\t", &count) != 1){
		printf("Error reading Variable 2\n");
		free(v);
		fclose(file);
		exit(1);
	}
	

	v->memberAccessList = 
		malloc(sizeof(struct SimpleVar*)  * count);
	for(int i = 0;i < count; i++){
		v->memberAccessList[i] = readVariable(file, debug);
	}
	
	v->count_memberAccessList = count;
	return v;
}
struct SimpleVar* readSimpleVar(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleVar(...)\n"); }
	
	struct SimpleVar* b = 
		malloc(sizeof(struct SimpleVar));
	
	int count;
	
	if(fscanf(file, "SimpleVar\t%s\t%d\t", b->name, &count) != 2){
		printf("Error reading SimpleVar\n");
		free(b);
		fclose(file);
		exit(1);
	}

	b->count_indices = count;
	b->indices = 
		malloc(sizeof(struct Expr*)* (b->count_indices+1));
	for(int i=0; i < b->count_indices; i++){
		b->indices[i] = readExpr(file, debug);
	}
	
	return b;
}
struct Term* readTerm(FILE* file, bool debug){
	
	if(debug){ printf("readTerm(...)\n"); }
	
	int kind;
	
	if(fscanf(file, "Term\t%d\t", &kind) != 1){
		printf("Error reading Term\n");
		fclose(file);
		fclose(file);
		exit(1);
	}
	
	struct Term* b = malloc(sizeof(struct Term));
	b->kind = kind;

	switch(b->kind){
		
		case 1: b->ptr.m1 = readBoolConst(file, debug); break;
		case 2: b->ptr.m2 = readIntConst(file, debug); break;
		case 3: b->ptr.m3 = readCharConst(file, debug); break;
		case 4: b->ptr.m4 = readMethodCall(file, debug); break;
		case 5: b->ptr.m5 = readExpr(file, debug); break;
		case 6: b->ptr.m6 = readVariable(file, debug); break;
		case 7: b->ptr.m7 = readFloatConst(file, debug); break;
		case 8: b->ptr.m8 = readStringConst(file, debug); break;
		
		default:
			printf("Error in readTerm\n");
			free(b);
			fclose(file);
			exit(1);
			break;
	}
	return b;
}

struct UnOpTerm* readUnOpTerm(FILE* file, bool debug){
	
	if(debug){ printf("readUnOpTerm(...)\n"); }
	
	int kind;
	if(fscanf(file, "UnOpTerm\t%d\t", &kind) != 1){
		printf("Error reading UnOpTerm\n");
		fclose(file);
		fclose(file);
		exit(1);
	}
	
	struct UnOpTerm* t = 
		malloc(sizeof(struct UnOpTerm));
	
	if(kind == 1){
		t->op = readOp(file, debug);
	}else{
		t->op = NULL;
	}
	
	t->term = readTerm(file, debug);
	
	return t;
}

struct Range* readRange(FILE* file, bool debug){
	
	if(debug){ printf("readRange(...)\n"); }
	
	if(fscanf(file, "Range\t") == EOF){
		printf("Error reading Range\n");
		fclose(file);
		exit(1);
	}
	
	struct Range* r = 
		malloc(sizeof(struct Range));
	
	r->start = readExpr(file, debug);
	r->end = readExpr(file, debug);
	
	return r;
}

//statementnodes
struct Stmt* readStmt(FILE* file, bool debug){
	
	if(debug){ printf("readStmt(...)\n"); }
	
	int kind;
	
	if(fscanf(file, "Stmt\t%d\t", &kind) != 1){
		printf("Error reading Stmt\n");
		fclose(file);
		exit(1);
	}
	
	struct Stmt* b = 
		malloc(sizeof(struct Stmt));
	b->kind = kind;

	switch(kind){
		case 0: b->ptr.m0 = readLoopStmt(file, debug);   break;
		case 1: b->ptr.m1 = readMethodCall(file, debug); break;
		case 2: b->ptr.m2 = readWhileStmt(file, debug);  break;
		case 3: b->ptr.m3 = readIfStmt(file, debug);     break;
		case 4: b->ptr.m4 = readRetStmt(file, debug);    break;
		case 5: b->ptr.m5 = readAssignStmt(file, debug); break;
		case 6: b->ptr.m6 = readBreakStmt(file, debug);  break;
		case 7: b->ptr.m7 = readForStmt(file, debug);  	 break;
		case 8: b->ptr.m8 = readSwitchStmt(file, debug); break;
		default:
			printf("Error in readStmt\n");
			free(b);
			fclose(file);
			exit(1);
			break;
	}
	return b;
}
struct IfStmt* readIfStmt(FILE* file, bool debug){
	
	if(debug){ printf("readIfStmt(...)\n"); }
	
	if(fscanf(file, "IfStmt\t") == EOF){
		printf("Error reading IfStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct IfStmt* v = 
		malloc(sizeof(struct IfStmt));
	
	v->condition = NULL;
	v->block = NULL;
	v->elseBlock = NULL;

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
	
	if(debug){ printf("readWhileStmt(...)\n"); }
	
	if(fscanf(file, "WhileStmt\t") == EOF){
		printf("Error reading WhileStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct WhileStmt* v = 
		malloc(sizeof(struct WhileStmt));

	v->condition = readExpr(file, debug);
	v->block = readStmtBlock(file, debug);
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct RetStmt* readRetStmt(FILE* file, bool debug){
	
	if(debug){ printf("readRetStmt(...)\n"); }
	
	if(fscanf(file, "RetStmt\t") == EOF){
		printf("Error reading RetStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct RetStmt* v = 
		malloc(sizeof(struct RetStmt));

	v->returnValue = readExpr(file, debug);
	return v;
}
struct AssignStmt* readAssignStmt(FILE* file, bool debug){
	
	if(debug){ printf("readAssignStmt(...)\n"); }
	
	int option;
	
	if(fscanf(file, "AssignStmt\t%d\t", &option) != 1){
		printf("Error reading AssignStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct AssignStmt* v = 
		malloc(sizeof(struct AssignStmt));

	if(option == 0){
		v->optType = NULL;
	}else if(option ==1){
		v->optType = readType(file, debug);
	}

	v->var = readVariable(file, debug);
	
	char assign_op[3];
	if(fscanf(file, "%s\t", assign_op) != 1){
		fclose(file);
		printf("Error in readAssignStmt\n");
		exit(1);
	}
	v->assign_op[2]='\0';
	strncpy(v->assign_op, assign_op, 2);
	
	v->expr = readExpr(file, debug);

	return v;
}
struct MethodCall* readMethodCall(FILE* file, bool debug){
	
	if(debug){ printf("readMethodCall(...)\n"); }
	
	struct MethodCall* v = 
		malloc(sizeof(struct MethodCall));

	if(
		fscanf(
			file, 
			"MethodCall\t%s\t%u\t", 
			v->methodName, 
			(unsigned int*)&(v->count_args)
		) != 2
	){
		printf("Error reading MethodCall\n");
		free(v);
		fclose(file);
		exit(1);
	}

	v->args = 
		malloc(sizeof(struct Expr*)*(v->count_args));
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = readExpr(file, debug);
	}
	return v;
}
struct LoopStmt* readLoopStmt(FILE* file, bool debug){
	
	if(debug){ printf("readLoopStmt(...)\n"); }
	
	if(fscanf(file, "LoopStmt\t") == EOF){
		printf("Error reading LoopStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct LoopStmt* v = 
		malloc(sizeof(struct LoopStmt));

	v->count = readExpr(file, debug);
	v->block = readStmtBlock(file, debug);
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct BreakStmt* readBreakStmt(FILE* file, bool debug){
	
	if(debug){ printf("readBreakStmt(...)\n"); }
	
	if(fscanf(file, "BreakStmt\t") == EOF){
		printf("Error reading BreakStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct BreakStmt* v = 
		malloc(sizeof(struct BreakStmt));
	
	return v;
}
struct ForStmt* readForStmt(FILE* file, bool debug){
	
	if(debug){ printf("readForStmt(...)\n"); }
	
	char indexName[DEFAULT_STR_SIZE];
	
	if(fscanf(file, "ForStmt\t%s\t", indexName) != 1){
		printf("Error reading ForStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct ForStmt* res = 
		malloc(sizeof(struct ForStmt));
	
	strncpy(res->indexName, indexName, DEFAULT_STR_SIZE);
	res->range = readRange(file, debug);
	res->block = readStmtBlock(file, debug);
	
	return res;
}
struct SwitchStmt* readSwitchStmt(FILE* file, bool debug){
	
	if(debug){ printf("readSwitchStmt(...)\n"); }
	
	if(fscanf(file, "SwitchStmt\t") == EOF){
		printf("Error reading SwitchStmt\n");
		fclose(file);
		exit(1);
	}
	
	struct SwitchStmt* res = 
		malloc(sizeof(struct SwitchStmt));
	
	res->var = readVariable(file, debug);
	
	int count;
	fscanf(file, "%d\t", &count);
	res->count_cases = count;
	
	res->cases = NULL;
	res->cases = 
		malloc(sizeof(struct CaseStmt*)* (res->count_cases));
	
	for(int i=0; i < res->count_cases; i++){
		
		res->cases[i] = readCaseStmt(file, debug);
	}
	
	return res;
}
struct CaseStmt* readCaseStmt(FILE* file, bool debug){
	
	if(debug){ printf("readCaseStmt(...)\n"); }
	
	int kind;
	
	if(fscanf(file, "CaseStmt\t%d\t", &kind) != 1){
		printf("Error reading Case\n");
		fclose(file);
		exit(1);
	}
	
	struct CaseStmt* res = 
		malloc(sizeof(struct CaseStmt));
	res->kind = kind;
	
	res->ptr.m1 = NULL;
	res->block = NULL;
	
	switch(kind){
		case 0: res->ptr.m1 = readBoolConst(file, debug); break;
		case 1: res->ptr.m2 = readCharConst(file, debug); break;
		case 2: res->ptr.m3 = readIntConst(file, debug); break;
		default:
			printf("Error in readCase\n");
			free(res);
			fclose(file);
			exit(1);
	}
	
	int hasBlock = 0;
	
	fscanf(file, "%d\t", &hasBlock);
	
	if(hasBlock == 1){
		
		res->block = readStmtBlock(file, debug);
	}
	
	return res;
}
// --- typenodes -------------------------
struct Type* readType(FILE* file, bool debug){
	
	if(debug){ printf("readType(...)\n"); }
	
	int kind;
	
	if(fscanf(file, "Type\t%d\t", &kind) != 1){
		printf("Error reading Type\n");
		fclose(file);
		exit(1);
	}
	
	struct Type* b = 
		malloc(sizeof(struct Type));
	
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;

	switch(kind){
		case 1: b->m1 = readBasicTypeWrapped(file, debug); break;
		case 2: b->m2 = readTypeParam(file, debug); break;
		case 3: b->m3 = readArrayType(file, debug); break;
		default:
			printf("Error in readType\n");
			free(b);
			fclose(file);
			exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
	return b;
}
struct SubrType* readSubrType(FILE* file, bool debug){
	
	if(debug){ printf("readSubrType(...)\n"); }
	
	if(fscanf(file, "SubrType\t") == EOF){
		printf("Error reading SubrType\n");
		fclose(file);
		exit(1);
	}
	
	struct SubrType* v = 
		malloc(sizeof(struct SubrType));

	v->returnType = readType(file, debug);
	
	if(
		fscanf(file, "%d\t%u\t", (int*)(&(v->hasSideEffects)), (unsigned int*)&(v->count_argTypes))
		!= 2
	){
		printf("Error reading SubrType 2\n");
		free(v);
		fclose(file);
		exit(1);
	}
	
	v->argTypes = 
		malloc(sizeof(struct Type*)*(v->count_argTypes));
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file, debug);
	}
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct SimpleType* readSimpleType(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleType(...)\n"); }
	
	struct SimpleType* v = 
		malloc(sizeof(struct SimpleType));
	
	//%30s ensures it does not read more than 30 chars
	if(fscanf(file, "SimpleType\t%30s\t", v->typeName) != 1){
		printf("Error reading SimpleType\n");
		free(v);
		fclose(file);
		exit(1);
	}
	
	int count=0;
	fscanf(file, "%d\t", &count);
	v->typeParamCount = count;
	if(v->typeParamCount > 0){
		v->typeParams = 
			malloc(sizeof(uint8_t)*(v->typeParamCount));
	}
	for(int i=0;i<v->typeParamCount;i++){
		int pIndex;
		fscanf(file, "%d\t", &pIndex);
		v->typeParams[i] = pIndex;
	}
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct ArrayType* readArrayType(FILE* file, bool debug){
	
	if(debug){ printf("readArrayType(...)\n"); }
	
	if(fscanf(file, "ArrayType\t") == EOF){
		printf("Error reading ArrayType\n");
		fclose(file);
		exit(1);
	}
	
	struct ArrayType* v = 
		malloc(sizeof(struct ArrayType));
	
	v->element_type = readType(file, debug);
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct TypeParam* readTypeParam(FILE* file, bool debug){
	
	if(debug){ printf("readTypeParam(...)\n"); }
	
	struct TypeParam* v = 
		malloc(sizeof(struct TypeParam));
	
	if(fscanf(file, "TypeParam\t%u\t", (unsigned int*)&(v->index)) != 1){
		printf("Error reading TypeParam\n");
		free(v);
		fclose(file);
		exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug){
	
	if(debug){ printf("readBasicTypeWrapped(...)\n"); }
	
	int kind = 0;
	
	if(fscanf(file, "BasicTypeWrapped\t%d\t", &kind) != 1){
		printf("Error reading BasicTypeWrapped\n");
		fclose(file);
		exit(1);
	}
	
	struct BasicTypeWrapped* v = 
		malloc(sizeof(struct BasicTypeWrapped));
	
	switch(kind){
		case 1: 
			v->simpleType = readSimpleType(file, debug);
			v->subrType = NULL;
			break;
		case 2: 
			v->simpleType = NULL;
			v->subrType = readSubrType(file, debug);
			break;
		default:
			printf("Error in readBasicTypeWrapped\n");
			free(v);
			fclose(file);
			exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
	return v;
}

