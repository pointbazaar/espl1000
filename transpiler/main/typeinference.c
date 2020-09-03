#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../ast/ast.h"
#include "../../ast/free_ast.h"

#include "typeinference.h"

//include the various symbol tables
#include "tables/symtable.h"
#include "tables/localvarsymtable.h"
#include "tables/subrsymtable.h"
#include "tables/structsymtable.h"

// -- subroutine declarations (private to this compile unit) --

struct Type* inferTypeTerm			(struct ST* st, struct Term* term);
struct Type* inferTypeUnOpTerm		(struct ST* st, struct UnOpTerm* t);
struct Type* inferTypeMethodCall	(struct ST* st, struct MethodCall* m);
struct Type* inferTypeVariable		(struct ST* st, struct Variable* v);
struct Type* inferTypeSimpleVar	(struct ST* st, struct SimpleVar* v);
// ------------------------------------------------------------
struct Type* typeFromStr(char* typeName);
char* typeToStrBasicTypeWrapped(struct BasicTypeWrapped* b);
// ------------------------------------------------------------

struct Type* inferTypeExpr(struct ST* st, struct Expr* expr){
	
	printf("inferTypeExpr(...)\n");
	
	if(expr->op == NULL){
		
		//only one term present
		return inferTypeUnOpTerm(st, expr->term1);
		
	}else{
		
		struct Type* type1Orig = inferTypeUnOpTerm(st, expr->term1);
		struct Type* type2Orig = inferTypeUnOpTerm(st, expr->term2);
		
		//string representations
		char* type1 = typeToStr(type1Orig);
		char* type2 = typeToStr(type2Orig);
		
		printf("type1: %s, type2: %s\n", type1, type2);
		
		if(strcmp(type1, type2) == 0){
			//both have the same type, 
			//and for most types i know
			//(not talking dependent types right now,
			// which shall be implemented later into smalldragon)
			//the resulting type if both are the same type
			//is exactly that type
			
			if(!(type2Orig->isInAST)){
				freeType(type2Orig);
			}
			return type1Orig;
		}else{
			
			//the types are different,
			//but maybe they are a float and a char
			//which would result in a float
			
			if(
				(strcmp(type1, "Float") == 0
				&& strcmp(type2, "Int") == 0)
				||
				(strcmp(type1, "Int") == 0
				&& strcmp(type2, "Float") == 0)
			){
				if(!(type1Orig->isInAST)){
					freeType(type1Orig);
				}
				if(!(type2Orig->isInAST)){
					freeType(type2Orig);
				}
				return typeFromStr("Float");
			}else{
				
				printf("Fatal Error in inferTypeExpr: could not infer type\n");
				exit(1);
				return NULL;
			}
		}
	}
}

struct Type* inferTypeTerm(struct ST* st, struct Term* t){
	
	if(t->m1 != NULL){ return typeFromStr("Bool"); }
	
	if(t->m2 != NULL){ return typeFromStr("Int"); }
	
	if(t->m3 != NULL){ return typeFromStr("Char"); }
	
	if(t->m4 != NULL){ return inferTypeMethodCall(st, t->m4); }
	
	if(t->m5 != NULL){ return inferTypeExpr(st, t->m5); }
	
	if(t->m6 != NULL){ return inferTypeVariable(st, t->m6); }
	
	if(t->m7 != NULL){ return typeFromStr("Float"); }
	
	if(t->m8 != NULL){ return typeFromStr("String"); }
	
	printf("Fatal Error in inferTypeTerm\n");
	exit(1);
	return NULL;
}

struct Type* inferTypeUnOpTerm(struct ST* st, struct UnOpTerm* t){
	
	// UnOpTerm means a term prefixed by an unary operator.
	// currently, such terms retain their original type,
	// so we can defer to that type
	
	return inferTypeTerm(st, t->term);
}

struct Type* inferTypeMethodCall(struct ST* st, struct MethodCall* m){

	//Subroutine Symbol Table
	struct SST* sst = st->sst;
	
	struct SSTLine* line = sst_get(sst, m->methodName);
	
	return line->returnType;
}

struct Type* inferTypeVariable(struct ST* st, struct Variable* v){
	
	struct Type* typeOfVar = inferTypeSimpleVar(st, v->simpleVar);
	
	if(v->count_memberAccessList == 0){
		
		//no member accesses
		return typeOfVar;
		
	}else{
		//struct STST* structSymTable = st->stst;
		
		//struct STSTLine* line = stst_get(structSymTable, typeOfVar);
		
		//TODO
		printf("local variable inference for variables with member access not yet implemented\n");
		exit(1);
		return NULL;
	}
}

struct Type* inferTypeSimpleVar(struct ST* st, struct SimpleVar* v){
	
	//as this variable is needed to infer the type
	//of another variable, it should have been initialized
	//before. so we can pull it's type from the LSVT
	
	struct LVSTLine* line = lvst_get(st->lvst, v->name);
	
	//if it has an index, we unwrap the type
	if(v->optIndex != NULL){
		
		//we get the element type of the type
		return line->type->m3->element_type;
	}
	return line->type;
}

struct Type* typeFromStr(char* typeName){
	//this method will only work for simple types
	struct Type* res = malloc(sizeof(struct Type));
	
	struct BasicTypeWrapped* btw = malloc(sizeof(struct BasicTypeWrapped));
	
	res->m1 = btw;
	res->m2 = NULL;
	res->m2 = NULL;
	res->isInAST = false;
	
	struct SimpleType* st = malloc(sizeof(struct SimpleType));
	
	strncpy(st->typeName, typeName, DEFAULT_STR_SIZE);
	
	btw->subrType = NULL;
	btw->simpleType = st;
	
	return res;
}

char* typeToStr(struct Type* t){
	
	if(t->m1 != NULL){
		return typeToStrBasicTypeWrapped(t->m1);
	}else{
		printf("(1)currently not implemented (in typeinference.c)\n");
		exit(1);
		return NULL;
	}
}

char* typeToStrBasicTypeWrapped(struct BasicTypeWrapped* b){
	
	if(b->simpleType != NULL){
		
		//convert NInt, PInt to Int
		char* typeName = b->simpleType->typeName;
		
		if(
			strcmp(typeName, "PInt") == 0
			|| strcmp(typeName, "NInt") == 0
		){
			return "Int";
		}
		
		return typeName;
		
	}else{
		printf("(2)currently not implemented (in typeinference.c)\n");
		exit(1);
		return NULL;
	}
}
