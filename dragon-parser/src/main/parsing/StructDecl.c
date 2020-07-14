#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StructDecl.h"
#include "StructMember.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"

struct StructDecl* makeStructDecl(struct TokenList* tokens, bool debug){

	if(debug){
		printf("makeStructDecl(...) from: %s\n", list_code(tokens, debug));
	}
	
	/*
	struct StructDecl{
		struct StructMember** members;
		int count_members;
	};
	struct StructMember{
		struct Type* type;
		char* name;
	};
	 */

	struct StructDecl* res = malloc(sizeof(struct StructDecl));
	
	res->members = malloc(sizeof(struct StructMember*)*1);
	res->count_members = 0;
	
	struct TokenList* copy = list_copy(tokens);

	if(list_size(copy) > 0){
	
		struct Token* next = list_head(copy);
		
		if(next->kind != STRUCT){return NULL;}
		list_consume(copy, 1);
			
		next = list_head(copy);
		if(next->kind != LCURLY){return NULL;}
		list_consume(copy, 1);
		
		struct StructMember* member;
		
		member = makeStructMember(copy, debug);
		while(member != NULL){
			
			res->members[res->count_members] = member;
			res->count_members++;
			
			//we just hope that the realloc will go through here
			res->members = realloc(
				res->members, 
				sizeof(struct StructMember*)*(res->count_members+1)
			);
			
			member = makeStructMember(copy, debug);
		}
		
		next = list_head(copy);
		if(next->kind != RCURLY){return NULL;}
		list_consume(copy, 1);
	}
	
	list_set(tokens, copy);

	return res;
}
