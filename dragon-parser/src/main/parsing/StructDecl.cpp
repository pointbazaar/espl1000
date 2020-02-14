#include "StructDecl.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/Token.hpp"

#include <stdio.h>

struct StructDecl* makeStructDecl(TokenList* tokens, bool debug){

	if(debug){
		printf("makeStructDecl(...)\n");
	}

	struct StructDecl* res = (struct StructDecl*)malloc(sizeof(struct StructDecl));

	//TODO
	res->members = NULL;
	res->count_members = 0;

	return res;
}