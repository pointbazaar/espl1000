#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SubrType.h"
#include "Type.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"

struct SubrType* makeSubrType2(struct Type* return_type, bool hasSideEffects){
	
	struct SubrType* res = malloc(sizeof(struct SubrType));

	res->returnType = return_type;
	res->hasSideEffects = hasSideEffects;
	res->count_argTypes = 0;

	return res;
}

struct SubrType* makeSubrType(struct TokenList* tokens, bool debug){

	if(debug){
		printf("SubrType(...)\n");
	}

	struct SubrType* res = malloc(sizeof(struct SubrType));

	res->argTypes = malloc(sizeof(struct Type*)*1);
	res->count_argTypes = 0;

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, LPARENS)){return NULL;}

	bool sucess_argument_types = true;

	bool fail = false;
	struct Type* mytype = makeType2(copy,debug);
	if(mytype == NULL){fail=true;}

	if(!fail){
		res->argTypes[res->count_argTypes++] = mytype;
		res->argTypes = realloc(res->argTypes,sizeof(struct Type*)*(res->count_argTypes + 1));

	}else{
		sucess_argument_types = false;
	}

	while (sucess_argument_types) {
		bool fail2 = false;
	
		struct TokenList* copy2 = list_copy(copy);

		if(!list_expect(copy2, COMMA)){fail2 = true;}
		
		if(!fail2){
			struct Type* mytype = makeType2(copy2,debug);
			if(mytype == NULL){fail2 = true;}

			if(!fail2){
				res->argTypes[res->count_argTypes] = mytype;
				res->count_argTypes++;

				size_t newsize = sizeof(struct Type*)*(res->count_argTypes + 1);
				res->argTypes = realloc(res->argTypes, newsize);

				list_set(copy, copy2);
			}
		}else {
			sucess_argument_types = false;
		}
		
		freeTokenListShallow(copy2);
	}

	if(!list_expect(copy, RPARENS)){return NULL;}

	if(!list_expect(copy, ARROW)){return NULL;}

	res->returnType = makeType2(copy,debug);
	if(res->returnType == NULL){return NULL;}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

void freeSubrType(struct SubrType* st){
	freeType(st->returnType);
	for(int i=0;i < st->count_argTypes; i++){
		freeType(st->argTypes[i]);
	}
	free(st);
}
