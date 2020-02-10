#include <vector>
#include <iostream>

#include "Type.hpp"
#include "../../commandline/TokenList.hpp"
#include "ArrayType.hpp"
#include "TypeParam.hpp"
#include "BasicTypeWrapped.hpp"

using namespace std;

Type::Type(BasicTypeWrapped* typeNode){
	this->m1 = typeNode;
}

Type::Type(ArrayType* typeNode){
	this->m3 = typeNode;
}

Type::Type(TypeParam* typeNode){
	this->m2 = typeNode;
}

Type::Type(TokenList tokens, bool debug){

	if(debug){
		cout << "Type(...)" << endl;
		cout << "from: " << tokens.code() << endl;
	}

	TokenList copy = tokens.copy();

	try {
		this->m3 = new ArrayType(copy,debug);
	} catch (std::string e) {
		try {
			this->m2 = new TypeParam(copy,debug);
		} catch (std::string e2) {
			this->m1 = new BasicTypeWrapped(copy,debug);
		}
	}


	tokens.set(copy);
}

