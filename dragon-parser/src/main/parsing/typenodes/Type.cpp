#include <vector>
#include <iostream>

#include "Type.hpp"
#include "../../commandline/TokenList.hpp"
#include "ArrayType.hpp"
#include "TypeParam.hpp"
#include "BasicTypeWrapped.hpp"

using namespace std;

struct Type* makeType(struct BasicTypeWrapped* typeNode){

	struct Type* res = (struct Type*)malloc(sizeof(struct Type));

	if(res == NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->m1 = typeNode;

	return res;
}

struct Type* makeType(struct ArrayType* typeNode){

	struct Type* res = (struct Type*)malloc(sizeof(struct Type));

	if(res == NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->m3 = typeNode;

	return res;
}

struct Type* makeType(struct TypeParam* typeNode){

	struct Type* res = (struct Type*)malloc(sizeof(struct Type));

	if(res == NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	res->m2 = typeNode;

	return res;
}

struct Type* makeType(TokenList* tokens, bool debug){

	if(debug){
		cout << "Type(...)" << "from: " << tokens->code() << endl;
	}

	struct Type* res = (struct Type*)malloc(sizeof(struct Type));

	if(res == NULL){
		printf("could not malloc.\n");
		exit(1);
	}

	TokenList copy = tokens->copy();

	try {
		res->m3 	= makeArrayType			(&copy,debug);
	} catch (std::string e) {
		try {
			res->m2 = makeTypeParam			(&copy,debug);
		} catch (std::string e2) {
			res->m1 = makeBasicTypeWrapped	(&copy,debug);
		}
	}


	tokens->set(copy);

	return res;
}

