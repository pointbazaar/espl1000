#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../ast/ast.h"
#include "../../ast/free_ast.h"
#include "code_gen/gen_c_types.h"
#include "typeinference.h"

//include the various symbol tables
#include "tables/symtable.h"
#include "tables/localvarsymtable.h"
#include "tables/subrsymtable.h"
#include "tables/structsymtable.h"

// -- subroutine declarations (private to this compile unit) --

struct Type* inferTypeTerm			(struct ST* st, struct Term* term, bool debug);
struct Type* inferTypeUnOpTerm		(struct ST* st, struct UnOpTerm* t, bool debug);
struct Type* inferTypeMethodCall	(struct ST* st, struct MethodCall* m, bool debug);
struct Type* inferTypeVariable		(struct ST* st, struct Variable* v, bool debug);
struct Type* inferTypeSimpleVar		(struct ST* st, struct SimpleVar* v, bool debug);
// ------------------------------------------------------------
char* typeToStrBasicTypeWrapped		(struct BasicTypeWrapped* b);
bool streq(char* str1, char* str2);
// ------------------------------------------------------------

//COMMENTS

	//[1]
	
	/*
		both have the same type, 
		and for most types i know
		(not talking dependent types right now,
		which shall be implemented later into smalldragon)
		the resulting type if both are the same type
		is exactly that type	
	*/

	//[2]
	
	/*
		the types are different,
		but maybe they are a float and a char
		which would result in a float
	*/
// -----------------

struct Type* inferTypeExpr(struct ST* st, struct Expr* expr, bool debug){
	
	if(debug){ printf("inferTypeExpr(...)\n"); }
	
	//only one term present ?
	if(expr->op == NULL) {
		
		return inferTypeUnOpTerm(st, expr->term1, debug);
	}
	
	struct Type* type1Orig = inferTypeUnOpTerm(st, expr->term1, debug);
	struct Type* type2Orig = inferTypeUnOpTerm(st, expr->term2, debug);
	
	char* type1 = typeToStr(type1Orig);
	char* type2 = typeToStr(type2Orig);
	
	if(debug) { printf("type1: %s, type2: %s\n", type1, type2); }
	
	if(streq(type1, type2)) { return type1Orig; /* [1] */ }
	
	if(isIntType(type1) && isIntType(type2)) {
		
		return typeFromStr(st, "Int");
	}
	
	//[2]
	const bool types_float_int = isIntType(type2) && streq(type1, "Float");
	const bool types_int_float = isIntType(type1) && streq(type2, "Float");
	
	if(types_float_int || types_int_float) {
	
		return typeFromStr(st, "Float");
	}
	
	printf("Fatal Error in inferTypeExpr: could not infer type\n");
	exit(1);
	
	return NULL;	
}

struct Type* inferTypeTerm(struct ST* st, struct Term* t, bool debug){
	
	switch(t->kind){
		
		case 1: return typeFromStr(st, "Bool"); 
	
		case 2: return typeFromStr(st, "Int"); 
		
		case 3: return typeFromStr(st, "Char"); 
		
		case 4: return inferTypeMethodCall(st, t->ptr.m4, debug); 
		
		case 5: return inferTypeExpr(st, t->ptr.m5, debug); 
		
		case 6: return inferTypeVariable(st, t->ptr.m6, debug); 
		
		case 7: return typeFromStr(st, "Float"); 
		
		case 8: return typeFromStr(st, "String"); 
		
		default:
			printf("Fatal Error in inferTypeTerm\n");
			exit(1);
	}
	
	return NULL;
}

struct Type* inferTypeUnOpTerm(struct ST* st, struct UnOpTerm* t, bool debug){
	
	// UnOpTerm means a term prefixed by an unary operator.
	// currently, such terms retain their original type,
	// so we can defer to that type
	
	return inferTypeTerm(st, t->term, debug);
}

struct Type* inferTypeMethodCall(struct ST* st, struct MethodCall* m, bool debug){

	//Subroutine Symbol Table
	struct SST* sst = st->sst;
	
	struct SSTLine* line = sst_get(sst, m->methodName);
	
	return line->returnType;
}

struct Type* inferTypeVariable(struct ST* st, struct Variable* v, bool debug){
	
	struct Type* typeOfVar = inferTypeSimpleVar(st, v->simpleVar, debug);
	
	if(v->count_memberAccessList == 0){
		
		//no member accesses
		return typeOfVar;
	}
	
	//TODO
	printf("local variable inference for variables with member access not yet implemented\n");
	exit(1);
	return NULL;

}

struct Type* inferTypeSimpleVar(struct ST* st, struct SimpleVar* v, bool debug){
	
	//as this variable is needed to infer the type
	//of another variable, it should have been initialized
	//before. so we can pull it's type from the LSVT
	
	//debug=true as param because
	//we do not get debug param here
	struct LVSTLine* line = lvst_get(st->lvst, v->name, debug);
	
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
		malloc(sizeof(struct BasicTypeWrapped));
	
	res->m1 = btw;
	res->m2 = NULL;
	res->m2 = NULL;
	
	struct SimpleType* simpleType = 
		malloc(sizeof(struct SimpleType));
	
	simpleType->typeParamCount = 0;
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
	}
	
	printf("(1)currently not implemented (in typeinference.c)\n");
	exit(1);
	return NULL;
}

bool streq(char* str1, char* str2){
	return strcmp(str1, str2) == 0;
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
		
	}
	
	printf("(2)currently not implemented (in typeinference.c)\n");
	exit(1);
	return NULL;	
}
