#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "typeinference/typeinfer.h"

static char* ERR_NO_ARRAYTYPE = 
	"Expected an ArrayType\n";

void print_exit(char* filename, struct ASTNode* node, char* msg){
	printf("[Typeinference]");
	printf("[Error]");
	printf("[%s:%d]", filename, node->line_num);
	printf("%s", msg); 
	exit(1);
}

struct Type* unwrap_indices(char* filename, struct Type* t, uint32_t count){
	
	uint32_t index_count = count;
	
	struct Type* res = t;
	
	while(index_count > 0){
		res = unwrap(filename, res);
		index_count--;
	}
	
	return res;
}

struct Type* unwrap(char* filename, struct Type* t){

	if(t->m3 == NULL){ 
		print_exit(filename, &(t->super), ERR_NO_ARRAYTYPE); 
	}
	
	return t->m3->element_type;
}
