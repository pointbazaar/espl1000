#include "UnOpTermTest.h"
#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"
#include "../../main/commandline/Token.h"
#include "../../main/parsing/UnOpTerm.h"
#include "../../main/parsing/Variable.h"
#include "../../main/parsing/SimpleVar.h"

#include <stdio.h>
#include <stdbool.h>

int test_unop_with(bool debug){
	if(debug){ printf("TEST: test_unop_with()\n"); }
	
	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(OPKEY, "!"));
	list_add(list, makeToken2(INTEGER,"4"));
	struct UnOpTerm* t = makeUnOpTerm(list,debug);
	
	freeTokenList(list);
	freeUnOpTerm(t);
	
	return (t != NULL)?1:0;
}

int test_unop_without(bool debug){
	if(debug){ printf("TEST: test_unop_without()\n"); }
	
	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER,"4"));
	struct UnOpTerm* t = makeUnOpTerm(list,debug);
	
	freeTokenList(list);
	freeUnOpTerm(t);
	
	return (t != NULL)?1:0;
}
