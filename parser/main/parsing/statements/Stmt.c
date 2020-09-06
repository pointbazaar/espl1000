#include <stdio.h>
#include <stdlib.h>

#include "Stmt.h"
#include "WhileStmt.h"
#include "ForStmt.h"
#include "LoopStmt.h"
#include "IfStmt.h"
#include "RetStmt.h"
#include "MethodCall.h"
#include "AssignStmt.h"
#include "BreakStmt.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../token/token.h"
#include "../../../../util/util.h"
#include "../../../../ast/free_ast.h"

// --- private subroutines ---

struct Stmt* initStmt();

void stmt_make_break(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_loop(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_while(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_if(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_return(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_for(struct Stmt* res, struct TokenList* copy, bool debug);
void stmt_make_other(struct Stmt* res, struct TokenList* copy, bool debug);
// ---------------------------

struct Stmt* makeStmt(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Stmt(...)\n");
	}
	
	if (list_size(tokens) == 0) { return NULL; }

	struct Stmt* res = initStmt();

	struct TokenList* copy = list_copy(tokens);

	struct Token* first = list_head(copy);
	
	switch(first->kind){
		case BREAK:
			stmt_make_break(res, copy, debug);
			break;
		case FOR:
			stmt_make_for(res, copy, debug);
			break;
		case LOOP:
			stmt_make_loop(res, copy, debug);
			break;
		case WHILE:
			stmt_make_while(res, copy, debug);
			break;
		case IF:
			stmt_make_if(res, copy, debug);
			break;
		case RETURN:
			stmt_make_return(res, copy, debug);
			break;
		default:
			stmt_make_other(res, copy, debug);
			break;
	}

	if(debug){
		printf("sucess parsing Stmt\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

struct Stmt* initStmt(){
	
	struct Stmt* res = smalloc(sizeof(struct Stmt));

	//init
	res->m0 = NULL;
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;
	
	return res;
}

void stmt_make_break(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->m6 = makeBreakStmt(copy, debug);
	
	if(res->m6 == NULL){
		printf("expected break stmt, but was:\n");
		list_print(copy);
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

void stmt_make_loop(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->m0 = makeLoopStmt(copy, debug);
		
	if(res->m0 == NULL){
		printf("expected loop stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		
		exit(1);
	}
}

void stmt_make_while(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->m2 = makeWhileStmt(copy,debug);
	if(res->m2 == NULL){
		printf("expected while stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		
		exit(1);
	}
}

void stmt_make_if(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->m3 = makeIfStmt(copy,debug);
	if(res->m3 == NULL){
		printf("expected if stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

void stmt_make_return(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->m4 = makeRetStmt(copy,debug);
	if(res->m4 == NULL){
		printf("expected return stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

void stmt_make_for(struct Stmt* res, struct TokenList* copy, bool debug){
	
	res->m7 = makeForStmt(copy,debug);
	if(res->m7 == NULL){
		printf("expected for stmt, but was:\n");
		list_print(copy);
		
		freeTokenListShallow(copy);
		free(res);
		exit(1);
	}
}

void stmt_make_other(struct Stmt* res, struct TokenList* copy, bool debug){
	
	//TODO: we have to figure something out here.
	//i don't want 'let' statements
	//because it just messes up the code and is
	//just bloat to write.
	//but parsing should be straightforward
	//and mostly deterministic
	//because it gives good performance and
	//to give good error messages
	
	res->m5 = makeAssignStmt(copy,debug);
	
	if(res->m5 == NULL){
		
		res->m1 = makeMethodCall(copy,debug);
		if(res->m1 == NULL){
			printf("expected method call, but was:\n");
			list_print(copy);
			
			freeTokenListShallow(copy);
			free(res);
			
			exit(1);
		}
		if(!list_expect(copy, SEMICOLON)){
			printf("expected ';', but was:\n");
			list_print(copy);
			
			freeTokenListShallow(copy);
			freeMethodCall(res->m1);
			free(res);
			
			exit(1);
		}
	}
}
