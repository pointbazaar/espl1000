#include <vector>
#include <variant>

#include "Type.hpp"
#include "../../commandline/TokenList.hpp"
#include "ArrayType.hpp"
#include "TypeParam.hpp"
#include "BasicTypeWrapped.hpp"


Type::Type(BasicTypeWrapped* typeNode){
	this->m1 = typeNode;
}

Type::Type(ArrayType* typeNode){
	this->m3 = typeNode;
}

Type::Type(TypeParam* typeNode){
	this->m2 = typeNode;
}

Type::Type(TokenList tokens){

	TokenList copy = tokens.copy();

	try {
		this->m3 = new ArrayType(copy);
	} catch (std::string e) {
		try {
			this->m2 = new TypeParam(copy);
		} catch (string e2) {
			this->m1 = new BasicTypeWrapped(copy);
		}
	}


	tokens.set(copy);
}

