#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "ForStmtTest.h"

#include "statements/ForStmt.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

#include "ast/util/free_ast.h"

void for_test1(bool debug) {

	if(debug){
		printf("TEST: for_test1\n");
	}

	struct TokenList* list = makeTokenList();
	
	list_add(list, makeToken2(FOR,"for"));

	list_add(list, makeToken2(ID, "i"));
	
	list_add(list, makeToken2(IN, "in"));
	
	list_add(list, makeToken2(INTEGER, "3"));
	list_add(list, makeToken2(RANGEOP,".."));
	list_add(list, makeToken2(ID,"b"));
	
	list_add(list, makeToken2(LCURLY,"{"));
	list_add(list, makeToken2(RCURLY,"}"));

	struct ForStmt* f = makeForStmt(list,debug);

	assert(list_size(list) == 0);
	
	assert(f != NULL);
	assert(f->range != NULL);
	assert(f->block != NULL);

	assert(f->block->count == 0);
	
	assert(f->range->start != NULL);
	assert(f->range->end != NULL);
	
	freeTokenList(list);
	freeForStmt(f);
}
