#include <vector>
#include <iostream>

#include "SubrType.hpp"
#include "Type.hpp"
#include "../../commandline/TokenList.hpp"
#include "../../commandline/Token.hpp"
#include "../../commandline/TokenKeys.hpp"

using namespace std;

struct SubrType* makeSubrType(struct Type* return_type, bool hasSideEffects){
	struct SubrType* res = (struct SubrType*)malloc(sizeof(struct SubrType));

	res->returnType = return_type;
	res->hasSideEffects = hasSideEffects;

	return res;
}

struct SubrType* makeSubrType(TokenList* tokens, bool debug){

	if(debug){
		cout << "SubrType(...)" << endl;
	}

	struct SubrType* res = (struct SubrType*)malloc(sizeof(struct SubrType));

	if(res==NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->argumentTypes = (struct Type**)malloc(sizeof(struct Type*)*1);

	if(res->argumentTypes == NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	TokenList copy = tokens->copy();

	copy.expect(LPARENS);

	bool sucess_argument_types = true;
	try {
		res->argumentTypes[res->count_argumentTypes++] = makeType(&copy,debug);
		res->argumentTypes = (struct Type**)realloc(res->argumentTypes,sizeof(struct Type*)*(res->count_argumentTypes));

	} catch (std::string e) {
		sucess_argument_types = false;
	}
	while (sucess_argument_types) {
		try {
			TokenList copy2 = copy.copy();

			copy2.expect(COMMA);
			
			res->argumentTypes[res->count_argumentTypes] = makeType(&copy2,debug);
			res->count_argumentTypes++;

			res->argumentTypes = (struct Type**)realloc(res->argumentTypes,sizeof(struct Type*)*(res->count_argumentTypes));

			copy.set(copy2);
		} catch (std::string e) {
			sucess_argument_types = false;
		}
	}

	copy.expect(RPARENS);

	copy.expect(ARROW);

	res->returnType = makeType(&copy,debug);

	tokens->set(copy);

	return res;
}

