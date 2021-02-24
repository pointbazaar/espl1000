#include "UnOpTermTest.h"

#include "../../main/commandline/TokenList.h"
#include "../../main/commandline/TokenKeys.h"

#include "UnOpTerm.h"
#include "var/Variable.h"
#include "var/SimpleVar.h"

#include "../../../token/token.h"
#include "../../../ast/free_ast.h"

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

int test_unop_with(bool debug){
	if(debug){ printf("TEST: test_unop_with()\n"); }
	
	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(OPKEY, "!"));
	list_add(list, makeToken2(INTEGER,"4"));
	
	struct UnOpTerm* t = makeUnOpTerm(list,debug);
	assert(t != NULL);
	
	freeTokenList(list);
	freeUnOpTerm(t);
	
	return 1;
}

int test_unop_without(bool debug){
	if(debug){ printf("TEST: test_unop_without()\n"); }
	
	struct TokenList* list = makeTokenList();
	list_add(list, makeToken2(INTEGER,"4"));
	
	struct UnOpTerm* t = makeUnOpTerm(list,debug);
	assert(t != NULL);
	
	freeTokenList(list);
	freeUnOpTerm(t);
	
	return 1;
}
