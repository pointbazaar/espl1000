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
	if(file == NULL){
		printf("could not open file: readAST(...)\n");
		exit(1);
	}

	struct AST_Whole_Program* ast = smalloc(sizeof(struct AST_Whole_Program));

	struct Namespace* ns = readNamespace(file, debug);
	
	ast->count_namespaces = 1;
	ast->namespaces = smalloc(sizeof(struct Namespace*));
	ast->namespaces[0] = ns;

	fclose(file);
	
	if(debug){ printf("readAST ~ done\n"); }
	
	return ast;
}

struct Namespace* readNamespace(FILE* file, bool debug){
	
	if(debug){ printf("readNamespace(...)\n"); }
	
	struct Namespace* ns = smalloc(sizeof(struct Namespace));

	int count = fscanf(file,
		"%s\t%s\t%hd\t", 
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
		ns->methods[i] = m;
	}
	
	//read structs
	count = fscanf(file, "%hd\t", &(ns->count_structs));
	
	if(count != 1){
		fclose(file);
		printf("error in readNamespace 3 ,%d\n",count);
		exit(1);
	}
	
	if(debug){
		printf("reading %hd Structs\n", ns->count_structs);
	}
	
	ns->structs = smalloc(sizeof(struct StructDecl*)*(ns->count_structs));
	for(int i=0;i < ns->count_structs; i++){
		
		struct StructDecl* s = readStructDecl(file, debug);
		ns->structs[i] = s;
	}
	
	if(debug){ printf("done\n"); }
	
	return ns;
}
struct Method* readMethod(FILE* file, bool debug){
	
	if(debug){
			printf("readMethod(...)\n");
	}
	
	struct Method* m = smalloc(sizeof(struct Method));

	if(fscanf(
		file,
		"Method\t%d\t%d\t%s\t",
		(int*)&(m->isPublic),
		(int*)&(m->hasSideEffects), 
		m->name) != 3
	){
		printf("Error reading Method \n");
		exit(1);
	}

	m->returnType = readType(file, debug);

	if(fscanf(file,"%hhd\t",&(m->count_args)) != 1){
		printf("Error reading Method 2\n");
		exit(1);
	}
	
	m->args = smalloc(sizeof(struct DeclArg*)*(m->count_args));

	for(int i = 0;i < m->count_args;i++){
		m->args[i] = readDeclArg(file, debug);
	}

	m->block = readStmtBlock(file, debug);
	
	if(debug){ printf("done\n"); }
	
	return m;
}
struct StructDecl* readStructDecl(FILE* file, bool debug){
	
	if(debug){
			printf("readStructDecl(...)\n");
	}
	
	struct StructDecl* res = smalloc(sizeof(struct StructDecl));
	
	if(
		fscanf(
			file, 
			"StructDecl\t%s\t%hd\t", 
			res->name, 
			&(res->count_members)
		) != 2
	){
		printf("Error reading StructDecl\n");
		exit(1);
	}
	
	res->members = smalloc(sizeof(struct StructMember*)*res->count_members);
	for(int i=0;i < res->count_members;i++){
		res->members[i] = readStructMember(file, debug);
	}
	
	if(debug){ printf("done\n"); }
	
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
	
	if(fscanf(file, "%s\t", res->name) != 1){
		printf("Error reading StructMember2\n");
		exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
	return res;
}

struct StmtBlock* readStmtBlock(FILE* file, bool debug){
	
	if(debug){
			printf("readStmtBlock(...)\n");
	}
	
	struct StmtBlock* block = smalloc(sizeof(struct StmtBlock));
	
	if(fscanf(file, "StmtBlock\t%hd\t", &(block->count)) != 1){
		printf("Error reading StmtBlock\n");
		exit(1);
	}
	
	block->stmts = smalloc(sizeof(struct Stmt*)* block->count);
	
	for(int i=0;i < block->count; i++){
		block->stmts[i] = readStmt(file, debug);
	}
	
	return block;
}

// -----------------------

struct DeclArg* readDeclArg(FILE* file, bool debug){
	
	if(debug){ printf("readDeclArg(...)\n"); }
	
	struct DeclArg* da = smalloc(sizeof(struct DeclArg));

	if(fscanf(file, "DeclaredArg\t") == EOF){
		printf("Error reading DeclaredArg\n");
		exit(1);
	}

	da->type = readType(file, debug);

	int option;
	if(fscanf(file, "%d\t", &option) != 1){
		printf("Error reading DeclaredArg 2\n");
		exit(1);
	}
	
	if(option == 0){
		da->has_name = false;
	}else if(option == 1){
		da->has_name = true;
		
		//do not read more than 19 chars
		if(fscanf(file, "%19s\t", da->name) != 1){
			printf("Error reading DeclaredArg 3\n");
			exit(1);
		}
	}else{
		printf("Error in readDeclArg\n");
		exit(1);
	}
	
	if(debug){ printf("done\n"); }

	return da;
}
struct Expr* readExpr(FILE* file, bool debug){
	
	if(debug){
			printf("readExpr(...)\n");
	}
	
	struct Expr* expr = smalloc(sizeof(struct Expr));
	
	if(fscanf(file,"Expr\t") == EOF){ 
		printf("Error reading Expr\n");
		exit(1);
	}

	expr->term1 = readTerm(file, debug);
	
	int option = 0;
	if(fscanf(file, "%d\t", &option) != 1){
		printf("Error reading Expr 2\n");
		exit(1);
	}
	
	if(option == 1){
		expr->op = NULL;
		expr->term2 = NULL;
	}else if (option == 0){
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
	
	if(fscanf(file, "Op\t%s\t", op->op) != 1){ 
		printf("Error reading Op\n");
		exit(1);
	}

	return op;
}
struct IntConst* readIntConst(FILE* file, bool debug){
	
	if(debug){
			printf("readIntConst(...)\n");
	}
	
	struct IntConst* ic = smalloc(sizeof(struct IntConst));
	
	if(fscanf(file, "IntConst\t%d\t", &(ic->value)) != 1){
		printf("Error reading IntConst\n");
		exit(1);
	}

	return ic;
}
struct BoolConst* readBoolConst(FILE* file, bool debug){
	
	if(debug){
			printf("readBoolConst(...)\n");
	}
	
	struct BoolConst* b = smalloc(sizeof(struct BoolConst));
	
	if(fscanf(file, "BoolConst\t%d\t", (int*)&(b->value)) != 1){
		printf("Error reading BoolConst\n");
		exit(1);
	}
	
	return b;
}
struct CharConst* readCharConst(FILE* file, bool debug){
	
	if(debug){
			printf("readCharConst(...)\n");
	}
	
	struct CharConst* b = smalloc(sizeof(struct CharConst));
	
	if(fscanf(file, "CharConst\t%c\t", &(b->value)) != 1){
		printf("Error reading CharConst\n");
		exit(1);
	}
	
	return b;
}
struct FloatConst* readFloatConst(FILE* file, bool debug){
	
	if(debug){
			printf("readFloatConst(...)\n");
	}
	
	struct FloatConst* ic = smalloc(sizeof(struct FloatConst));
	
	if(fscanf(file, "FloatConst\t%f\t", &(ic->value)) != 1){
		printf("Error reading FloatConst\n");
		exit(1);
	}
	
	return ic;
}
struct StringConst* readStringConst(FILE* file, bool debug){
	
	if(debug){ printf("readStringConst(...)\n"); }
	
	struct StringConst* s = smalloc(sizeof(struct StringConst));
	
	if(fscanf(file, "StringConst\t%s\t", s->value) != 1){
		printf("Error reading StringConst\n");
		exit(1);
	}
	
	return s;
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
	
	if(fscanf(file, "%d\t", &memberAccessCount) != 1){
		printf("Error reading Variable 2\n");
		exit(1);
	}

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
	
	int option;
	
	if(fscanf(file, "SimpleVar\t%s\t%d\t", b->name, &option) != 2){
		printf("Error reading SimpleVar\n");
		exit(1);
	}

	if(option == 1){
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
	b->m8 = NULL;
	
	int kind;
	
	if(fscanf(file, "Term\t%d\t", &kind) != 1){
		printf("Error reading Term\n");
		exit(1);
	}

	switch(kind){
		
		case 1: b->m1 = readBoolConst(file, debug); break;
		case 2: b->m2 = readIntConst(file, debug); break;
		case 3: b->m3 = readCharConst(file, debug); break;
		case 4: b->m4 = readMethodCall(file, debug); break;
		case 5: b->m5 = readExpr(file, debug); break;
		case 6: b->m6 = readVariable(file, debug); break;
		case 7: b->m7 = readFloatConst(file, debug); break;
		case 8: b->m8 = readStringConst(file, debug); break;
		
		default:
			printf("Error in readTerm\n");
			exit(1);
			break;
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
	
	int kind;
	
	if(fscanf(file, "Stmt\t%d\t", &kind) != 1){
		printf("Error reading Stmt\n");
		exit(1);
	}

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
	
	if(debug){ printf("done\n"); }
	
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
	
	int option;
	
	if(fscanf(file, "AssignStmt\t%d\t", &option) != 1){
		printf("Error reading AssignStmt\n");
		exit(1);
	}

	if(option == 0){
		v->optType = NULL;
	}else if(option ==1){
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

	if(
		fscanf(
			file, 
			"MethodCall\t%s\t%hhd\t", 
			v->methodName, 
			&(v->count_args)
		) != 2
	){
		printf("Error reading MethodCall\n");
		exit(1);
	}

	v->args = smalloc(sizeof(struct Expr*)*(v->count_args));
	for(int i=0;i < (v->count_args);i++){
		v->args[i] = readExpr(file, debug);
	}
	return v;
}

//typenodes
struct Type* readType(FILE* file, bool debug){
	
	if(debug){ printf("readType(...)\n"); }
	
	struct Type* b = smalloc(sizeof(struct Type));
	
	b->m1 = NULL;
	b->m2 = NULL;
	b->m3 = NULL;
	
	int kind;
	
	if(fscanf(file, "Type\t%d\t", &kind) != 1){
		printf("Error reading Type\n");
		exit(1);
	}

	switch(kind){
		case 1: b->m1 = readBasicTypeWrapped(file, debug); break;
		case 2: b->m2 = readTypeParam(file, debug); break;
		case 3: b->m3 = readArrayType(file, debug); break;
		default:
			printf("Error in readType\n");
			exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
	return b;
}
struct SubrType* readSubrType(FILE* file, bool debug){
	
	if(debug){ printf("readSubrType(...)\n"); }
	
	struct SubrType* v = smalloc(sizeof(struct SubrType));
	
	if(fscanf(file, "SubrType\t") == EOF){
		printf("Error reading SubrType\n");
		exit(1);
	}

	v->returnType = readType(file, debug);
	
	if(
		fscanf(file, "%d\t%hhd\t", (int*)(&(v->hasSideEffects)), &(v->count_argTypes))
		!= 2
	){
		printf("Error reading SubrType 2\n");
		exit(1);
	}
	
	v->argTypes = smalloc(sizeof(struct Type*)*(v->count_argTypes));
	for(int i=0;i < (v->count_argTypes); i++){
		v->argTypes[i] = readType(file, debug);
	}
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct SimpleType* readSimpleType(FILE* file, bool debug){
	
	if(debug){ printf("readSimpleType(...)\n"); }
	
	struct SimpleType* v = smalloc(sizeof(struct SimpleType));
	
	//%30s ensures it does not read more than 30 chars
	if(fscanf(file, "SimpleType\t%30s\t", v->typeName) != 1){
		printf("Error reading SimpleType\n");
		exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
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
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct TypeParam* readTypeParam(FILE* file, bool debug){
	
	if(debug){
			printf("readTypeParam(...)\n");
	}
	
	struct TypeParam* v = smalloc(sizeof(struct TypeParam));
	
	if(fscanf(file, "TypeParam\t%hhd\t", &(v->index)) != 1){
		printf("Error reading TypeParam\n");
		exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
	return v;
}
struct BasicTypeWrapped* readBasicTypeWrapped(FILE* file, bool debug){
	
	if(debug){
			printf("readBasicTypeWrapped(...)\n");
	}
	
	struct BasicTypeWrapped* v = smalloc(sizeof(struct BasicTypeWrapped));
	
	int kind = 0;
	
	if(fscanf(file, "BasicTypeWrapped\t%d\t", &kind) != 1){
		printf("Error reading BasicTypeWrapped\n");
		exit(1);
	}
	
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
			exit(1);
	}
	
	if(debug){ printf("done\n"); }
	
	return v;
}

