#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ast/ast.h"
#include "ast/util/free_ast.h"
#include "ast/util/str_ast.h"

#include "code_gen/gen_c_types.h"
#include "typeinference/typeinference.h"
#include "typeinference/type_str.h"

//include the various symbol tables
#include "tables/symtable.h"
#include "tables/lvst.h"
#include "tables/sst.h"
#include "tables/stst.h"

// -- subroutine declarations (private to this compile unit) --

struct Type* inferTypeTerm			(struct ST* st, struct Term* term, bool debug);
struct Type* inferTypeUnOpTerm		(struct ST* st, struct UnOpTerm* t, bool debug);
struct Type* inferTypeMethodCall	(struct ST* st, struct MethodCall* m, bool debug);
struct Type* inferTypeVariable		(struct ST* st, struct Variable* v, bool debug);
struct Type* inferTypeSimpleVar		(struct ST* st, struct SimpleVar* v, bool debug);
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
	
	char* type1 = strType(type1Orig);
	char* type2 = strType(type2Orig);
	
	if(debug) { printf("type1: %s, type2: %s\n", type1, type2); }
	
	if(strcmp(type1, type2) == 0) { return type1Orig; /* [1] */ }
	
	if(isIntType(type1Orig) && isIntType(type2Orig)) {
		
		return typeFromStr(st, "int", true, true);
	}
	
	//[2]
	const bool types_float_int = isIntType(type2Orig) && strcmp(type1, "float") == 0;
	const bool types_int_float = isIntType(type1Orig) && strcmp(type2, "float") == 0;
	
	if(types_float_int || types_int_float) {
	
		return typeFromStr(st, "float", true, false);
	}
	
	printf("Fatal Error in inferTypeExpr: could not infer type\n");
	exit(1);
	
	return NULL;	
}

struct Type* inferTypeTerm(struct ST* st, struct Term* t, bool debug){
	
	switch(t->kind){
		
		case  1: return typeFromStr(st, "bool", true, false); 
		case  2: return typeFromStr(st, "int", true, true); 
		case  3: return typeFromStr(st, "char", true, false); 
		case  4: return inferTypeMethodCall(st, t->ptr.m4, debug); 
		case  5: return inferTypeExpr(st, t->ptr.m5, debug); 
		case  6: return inferTypeVariable(st, t->ptr.m6, debug); 
		case  7: return typeFromStr(st, "float", true, false); 
		case  8: return typeFromStr(st, "String", false, false); 
		case  9: return typeFromStr(st, "int", true, true);
		case 10: return typeFromStr(st, "int", true, true);
		
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
	
	if(v->memberAccess == NULL){
		
		//no member accesses
		return typeOfVar;
	}

	char* typeName = strType(typeOfVar);

	char* memberName = v->memberAccess->simpleVar->name;

	struct Type* type = stst_get_member_type(st->stst, typeName, memberName);


	//TODO: implement for the recursive case(further member access)
	//TODO: implement for the case of indices on the members
	if(v->memberAccess->simpleVar->count_indices != 0){

		printf("struct member access, then access with indices not implemented yet\n");
		printf("(typeinference.c)\n");
		exit(1);
	}
	
	return type;

}

struct Type* inferTypeSimpleVar(struct ST* st, struct SimpleVar* v, bool debug){
	
	//as this variable is needed to infer the type
	//of another variable, it should have been initialized
	//before. so we can pull it's type from the LSVT
	
	//debug=true as param because
	//we do not get debug param here
	struct LVSTLine* line = lvst_get(st->lvst, v->name);
	
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


