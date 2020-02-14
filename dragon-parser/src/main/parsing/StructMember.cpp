#include "StructMember.hpp"
#include "../commandline/TokenList.hpp"
#include "../commandline/TokenKeys.hpp"
#include "../commandline/Token.hpp"

#include <stdio.h>

struct StructMember* makeStructMember(TokenList* tokens, bool debug){

	if(debug){
		printf("makeStructMember(...)\n");
	}

	struct StructMember* res = (struct StructMember*)malloc(sizeof(struct StructMember));

	//TODO
	
	return res;
}