#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Namespace.h"
#include "Method.h"
#include "StructDecl.h"
#include "../commandline/TokenList.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"

struct Namespace* makeNamespace(struct TokenList* tokens, char* name, bool debug) {

	if (debug) {
		printf("Namespace(...) from: %s\n", list_code(tokens, debug));
	}

	struct Namespace* res = malloc(sizeof(struct Namespace));

	if(res==NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->count_methods = 0;
	res->count_structs = 0;

	res->methods = malloc(sizeof(struct Method*)*1);
	res->structs = malloc(sizeof(struct StructDecl*)*1);

	strcpy(res->srcPath, "/dev/null");
	strcpy(res->name, name);
	
	struct TokenList* copy = list_copy(tokens);
	
	ns_parse_structs(res, copy, debug);
	
	ns_parse_methods(res, copy, debug);

	if(debug){
		printf("done parsing Namespace Node\n");
	}

	list_set(tokens, copy);

	return res;
}

void ns_parse_methods(struct Namespace* res, struct TokenList* copy, bool debug){
	
	if (list_size(copy) > 0) {

		struct Token* next = list_head(copy);

		while (next->kind == FN) {
			struct Method* m = makeMethod(copy,debug);
			if(m == NULL){
				printf("parsing error, expected a method, but got %s\n", list_code(copy,debug));
				exit(1);
			}

			res->methods[res->count_methods++] = m;
			res->methods = realloc(res->methods,sizeof(struct Method*)*(res->count_methods+1));

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

void freeNamespace(struct Namespace* ns){
	
	for(int i=0;i < ns->count_methods; i++){
		freeMethod(ns->methods[i]);
	}
	for(int i=0;i < ns->count_structs; i++){
		freeStructDecl(ns->structs[i]);
	}
	free(ns);
}
