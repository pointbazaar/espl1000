#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "typeinference/typeinfer.h"

static char* ERR_NO_ARRAYTYPE = "Expected an ArrayType\n";

static void print_exit(char *msg) {
	printf("[Typeinference]");
	printf("[Error]");
	printf("%s", msg); 
	exit(1);
}

struct Type *unwrap_indices(struct Type *t, uint32_t count) {
	
	uint32_t index_count = count;
	
	struct Type* res = t;
	
	while(index_count > 0){
		res = unwrap(res);
		index_count--;
	}
	
	return res;
}

struct Type *unwrap(struct Type *t) {

	if(t->m3 == NULL){
        print_exit(ERR_NO_ARRAYTYPE);
	}
	
	return t->m3->element_type;
}
