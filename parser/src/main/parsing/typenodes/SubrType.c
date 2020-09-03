#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SubrType.h"
#include "Type.h"
#include "../../commandline/TokenList.h"
#include "../../../../../token/token.h"
#include "../../commandline/TokenKeys.h"
#include "../../../../../util/util.h"
#include "../../../../../ast/free_ast.h"

struct SubrType* makeSubrType2(struct Type* return_type, bool hasSideEffects){
	
	struct SubrType* res = smalloc(sizeof(struct SubrType));

	res->returnType = return_type;
	res->hasSideEffects = hasSideEffects;
	res->count_argTypes = 0;

	return res;
}

struct SubrType* makeSubrType(struct TokenList* tokens, bool debug){

	if(debug){
		printf("SubrType(...)\n");
	}

	struct SubrType* res = smalloc(sizeof(struct SubrType));

	res->argTypes = smalloc(sizeof(struct Type*)*1);
	res->count_argTypes = 0;

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, LPARENS)){
		freeTokenListShallow(copy);
		free(res->argTypes);
		free(res);
		return NULL;
	}

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

	if(!list_expect(copy, RPARENS)){
		freeTokenListShallow(copy);
		free(res->argTypes);
		free(res);
		return NULL;
	}

	if(!list_expect(copy, ARROW)){
		freeTokenListShallow(copy);
		free(res->argTypes);
		free(res);
		return NULL;
	}

	res->returnType = makeType2(copy,debug);
	if(res->returnType == NULL){
		freeTokenListShallow(copy);
		free(res->argTypes);
		free(res);
		return NULL;
	}

	list_set(tokens, copy);
	freeTokenListShallow(copy);

	return res;
}

