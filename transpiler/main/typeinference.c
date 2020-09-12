#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../ast/ast.h"
#include "../../ast/free_ast.h"
#include "../../util/util.h"

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
				return typeFromStr(st, "Float");
			}else{
				
				printf("Fatal Error in inferTypeExpr: could not infer type\n");
				exit(1);
				return NULL;
			}
		}
	}
}

struct Type* inferTypeTerm(struct ST* st, struct Term* t){
	
	switch(t->kind){
		case 1: return typeFromStr(st, "Bool"); 
	
		case 2: return typeFromStr(st, "Int"); 
		
		case 3: return typeFromStr(st, "Char"); 
		
		case 4: return inferTypeMethodCall(st, t->ptr.m4); 
		
		case 5: return inferTypeExpr(st, t->ptr.m5); 
		
		case 6: return inferTypeVariable(st, t->ptr.m6); 
		
		case 7: return typeFromStr(st, "Float"); 
		
		case 8: return typeFromStr(st, "String"); 
		
		default:
			printf("Fatal Error in inferTypeTerm\n");
			exit(1);
	}
	
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
	
	//debug=true as param because
	//we do not get debug param here
	struct LVSTLine* line = lvst_get(st->lvst, v->name, true);
	
	//if it has an index, we unwrap the type
	if(v->count_indices != 0){
		//we get the element type of the type
		//and repeat that as many times as there
		//are indices
		
		struct Type* elem_type = line->type->m3->element_type;
		
		for(int i=1;i < v->count_indices; i++){
			elem_type = elem_type->m3->element_type;
		}
		
		return elem_type;
	}
	return line->type;
}

struct Type* typeFromStr(struct ST* st, char* typeName){
	//this method will only work for simple types
	struct Type* res = malloc(sizeof(struct Type));
	
	struct BasicTypeWrapped* btw = 
		smalloc(sizeof(struct BasicTypeWrapped));
	
	res->m1 = btw;
	res->m2 = NULL;
	res->m2 = NULL;
	
	struct SimpleType* simpleType = 
		smalloc(sizeof(struct SimpleType));
	
	strncpy(simpleType->typeName, typeName, DEFAULT_STR_SIZE);
	
	btw->subrType = NULL;
	btw->simpleType = simpleType;
	
	//register the pointer in SymbolTable 'st'
	//so it can manage that memory,
	//as this pointer is not part of the AST Tree
	
	if(st->inferredTypesCount >= st->inferredTypesCapacity){
		printf("Fatal Error (in typeinference.c)\n");
		exit(1);
	}else{
		st->inferredTypes[st->inferredTypesCount] = res;
		st->inferredTypesCount += 1;
	}
	
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
