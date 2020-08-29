#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../ast/ast.h"

#include "typeinference.h"

//subroutine declarations

//TODO: declare the utility subroutines
//private to this file

char* inferTypeTerm(struct LVST* lvst, struct Term* term);
char* inferTypeUnOpTerm(struct LVST* lvst, struct UnOpTerm* t);
char* inferTypeMethodCall(struct LVST* lvst, struct MethodCall* m);
char* inferTypeVariable(struct LVST* lvst, struct Variable* v);
char* inferTypeSimpleVar(struct LVST* lvst, struct SimpleVar* v);
// -----------------------

char* inferTypeExpr(struct LVST* lvst, struct Expr* expr){
	
	if(expr->op == NULL){
		
		//only one term present
		return inferTypeUnOpTerm(lvst, expr->term1);
		
	}else{
		
		char* type1 = inferTypeUnOpTerm(lvst, expr->term1);
		char* type2 = inferTypeUnOpTerm(lvst, expr->term2);
		
		if(strcmp(type1, type2) == 0){
			//both have the same type, 
			//and for most types i know
			//(not talking dependent types right now,
			// which shall be implemented later into smalldragon)
			//the resulting type if both are the same type
			//is exactly that type
			
			return type1;
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
				return "Float";
			}else{
				
				printf("Fatal Error in inferTypeExpr: could not infer type\n");
				exit(1);
				return NULL;
			}
		}
	}
}

char* inferTypeTerm(struct LVST* lvst, struct Term* t){
	
	if(t->m1 != NULL){ return "Bool"; }
	
	if(t->m2 != NULL){ return "Int"; }
	
	if(t->m3 != NULL){ return "Char"; }
	
	if(t->m4 != NULL){ return inferTypeMethodCall(lvst, t->m4); }
	
	if(t->m5 != NULL){ return inferTypeExpr(lvst, t->m5); }
	
	if(t->m6 != NULL){ return inferTypeVariable(lvst, t->m6); }
	
	if(t->m7 != NULL){ return "Float"; }
	
	if(t->m8 != NULL){ return "String"; }
	
	printf("Fatal Error in inferTypeTerm\n");
	exit(1);
	return NULL;
}

char* inferTypeUnOpTerm(struct LVST* lvst, struct UnOpTerm* t){
	
	// UnOpTerm means a term prefixed by an unary operator.
	// currently, such terms retain their original type,
	// so we can defer to that type
	
	return inferTypeTerm(lvst, t->term);
}

char* inferTypeMethodCall(struct LVST* lvst, struct MethodCall* m){

	//TODO
	
	//MethodCall
	//TODO: subroutine symbol table is needed
	//here to determine the type of value
	//returned by the method
	//SubrSymTable
	
	printf("local variable inference for method calls not yet implemented\n");
	exit(1);
	return NULL;
}

char* inferTypeVariable(struct LVST* lvst, struct Variable* v){
	
	if(v->count_memberAccessList == 0){
		
		//no member accesses
		return inferTypeSimpleVar(lvst, v->simpleVar);
		
	}else{
		//TODO
		printf("local variable inference for variables with member access not yet implemented\n");
		exit(1);
		return NULL;
	}
}

char* inferTypeSimpleVar(struct LVST* lvst, struct SimpleVar* v){
	
	//as this variable is needed to infer the type
	//of another variable, it should have been initialized
	//before. so we can pull it's type from the LSVT
	
	struct LVSTLine* line = lvst_get(lvst, v->name);
	
	//if it has an index, we unwrap the type
	if(v->optIndex != NULL){
		
		char* res = malloc(sizeof(char)*32);
		strncpy(res, (line->type)+1, strlen(line->type)-2);
		return res;
	}
	return line->type;
}
