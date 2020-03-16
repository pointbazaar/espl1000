#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SubrType.h"
#include "Type.h"
#include "../../commandline/TokenList.h"
#include "../../commandline/Token.h"
#include "../../commandline/TokenKeys.h"
#include "../../commandline/smalloc.h"

struct SubrType* makeSubrType(struct Type* return_type, bool hasSideEffects){
	
	struct SubrType* res = smalloc(sizeof(struct SubrType));

	res->returnType = return_type;
	res->hasSideEffects = hasSideEffects;
	res->count_argumentTypes = 0;

	return res;
}

struct SubrType* makeSubrType(struct TokenList* tokens, bool debug){

	if(debug){
		printf("SubrType(...)\n");
	}

	struct SubrType* res = smalloc(sizeof(struct SubrType));

	res->argumentTypes = smalloc(sizeof(struct Type*)*1);
	res->count_argumentTypes = 0;

	struct TokenList* copy = list_copy(tokens);

	if(!list_expect(copy, LPARENS)){return NULL;}

	bool sucess_argument_types = true;

	bool fail = false;
	struct Type* mytype = makeType(copy,debug);
	if(mytype == NULL){fail=true;}

	if(!fail){
		res->argumentTypes[res->count_argumentTypes++] = mytype;
		res->argumentTypes = realloc(res->argumentTypes,sizeof(struct Type*)*(res->count_argumentTypes));

	}else{
		sucess_argument_types = false;
	}

	while (sucess_argument_types) {
		bool fail2 = false;
	
		struct TokenList* copy2 = list_copy(copy);

		if(!list_expect(copy2, COMMA)){fail2 = true;}
		
		if(!fail2){
			struct Type* mytype = makeType(copy2,debug);
			if(mytype == NULL){fail2 = true;}

			if(!fail2){
				res->argumentTypes[res->count_argumentTypes] = mytype;
				res->count_argumentTypes++;

				res->argumentTypes = realloc(res->argumentTypes,sizeof(struct Type*)*(res->count_argumentTypes));

				list_set(copy, copy2);
			}
		}else {
			sucess_argument_types = false;
		}
	}

	if(!list_expect(copy, RPARENS)){return NULL;}

	if(!list_expect(copy, ARROW)){return NULL;}

	res->returnType = makeType(copy,debug);
	if(res->returnType == NULL){return NULL;}

	list_set(tokens, copy);

	return res;
}

