#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "StructMember.h"
#include "../commandline/TokenList.h"
#include "../commandline/TokenKeys.h"
#include "../commandline/Token.h"
#include "../commandline/smalloc.h"

struct StructMember* makeStructMember(struct TokenList* tokens, bool debug){

	if(debug){
		printf("makeStructMember(...)\n");
	}

	struct StructMember* res = smalloc(sizeof(struct StructMember));

	//TODO
	
	return res;
}