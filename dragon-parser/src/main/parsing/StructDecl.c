#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StructDecl.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "../commandline/smalloc.h"

struct StructDecl* makeStructDecl(struct TokenList* tokens, bool debug){

	if(debug){
		printf("makeStructDecl(...)\n");
	}

	struct StructDecl* res = smalloc(sizeof(struct StructDecl));

	//TODO
	res->members = NULL;
	res->count_members = 0;

	return res;
}