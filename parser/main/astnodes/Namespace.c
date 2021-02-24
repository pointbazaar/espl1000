#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Namespace.h"
#include "Method.h"
#include "struct/StructDecl.h"
#include "../commandline/TokenList.h"
#include "../../../token/token.h"
#include "../commandline/TokenKeys.h"
#include "../../../ast/free_ast.h"

struct Namespace* makeNamespace(struct TokenList* tokens, char* name, bool debug) {

	if (debug) {
		printf("Namespace(...) from: ");
		list_print(tokens);
	}

	struct Namespace* res = malloc(sizeof(struct Namespace));

	//because of valgrind
	//it will complain about uninitialized bytes otherwise
	memset(res, 0, sizeof(struct Namespace));

	res->count_methods = 0;
	res->count_structs = 0;
	
	res->capacity_methods = 5;
	res->capacity_structs = 5;

	res->methods = malloc(sizeof(struct Method*) * res->capacity_methods);
	res->structs = malloc(sizeof(struct StructDecl*) * res->capacity_structs);

	strncpy(res->srcPath, tokens->relPath, DEFAULT_STR_SIZE);
	strncpy(res->name, name, DEFAULT_STR_SIZE);
	
	struct TokenList* copy = list_copy(tokens);
	
	ns_parse_structs(res, copy, debug);
	
	ns_parse_methods(res, copy, debug);

	if(debug){
		printf("done parsing Namespace Node\n");
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void ns_parse_methods(struct Namespace* res, struct TokenList* copy, bool debug){
	
	if (list_size(copy) > 0) {

		struct Token* next = list_head(copy);

		while (next->kind == FN) {
			struct Method* m = makeMethod(copy,debug);
			if(m == NULL){
				printf("parsing error, expected a method, but got:\n");
				list_print(copy);
				
				freeNamespace(res);
				exit(1);
			}

			res->methods[res->count_methods] = m;
			res->count_methods++;
			
			if(res->count_methods >= res->capacity_methods){
				res->capacity_methods *= 2;
				res->methods = realloc(res->methods,sizeof(struct Method*)*(res->capacity_methods));
			}

			if (list_size(copy) > 0) {
				next = list_head(copy);
			} else {
				break;
			}
		}
	}
}
void ns_parse_structs(struct Namespace* res, struct TokenList* copy, bool debug){
	
	if(list_size(copy)>0) {

		struct Token* next = list_head(copy);

		while (next->kind == STRUCT) {
			struct StructDecl* sd = makeStructDecl(copy,debug);
			if(sd == NULL){
				printf("parsing error, expected a struct, but got %s\n", list_code(copy,debug));
				exit(1);
			}

			res->structs[res->count_structs] = sd;
			res->count_structs++;
			
			res->structs = realloc(res->structs,sizeof(struct StructDecl*)*(res->count_structs+1));

			if (list_size(copy) > 0) {
				next = list_head(copy);
			} else {
				break;
			}
		}
	}
}


