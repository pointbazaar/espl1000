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
	
	if(list_size(copy)>0) {

		struct Token* next_struct = list_head(copy);
		if(next_struct == NULL){return NULL;}

		while (next_struct->kind == STRUCT) {
			struct StructDecl* mystructdecl = makeStructDecl(copy,debug);
			if(mystructdecl == NULL){return NULL;}

			res->structs[res->count_structs] = mystructdecl;
			res->count_structs++;
			res->structs = realloc(res->structs,sizeof(struct StructDecl*)*(res->count_structs+1));

			if (list_size(copy) > 0) {
				next_struct = list_head(copy);
			} else {
				break;
			}
		}
	}
	

	//it is be nice to have prefix 'fn' before a function
	//to make parsing easier.
	//this does not add much boilerplate to the syntax
	//and would probably make the parser faster
	
	if (list_size(copy) > 0) {

		struct Token* next_subr = list_head(copy);

		while (next_subr->kind == FN) {
			struct Method* mthd = makeMethod(copy,debug);
			if(mthd == NULL){return NULL;}

			res->methods[res->count_methods++] = mthd;
			res->methods = realloc(res->methods,sizeof(struct Method*)*(res->count_methods+1));

			if (list_size(copy) > 0) {
				next_subr = list_head(copy);
			} else {
				break;
			}
		}
	}

	if(debug){
		printf("done parsing Namespace Node\n");
	}

	list_set(tokens, copy);

	return res;
}

