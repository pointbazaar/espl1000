#include <string>
#include "BaseToken.hpp"

using namespace std;

BaseToken::BaseToken(int kind){
	this->kind = kind;
}

BaseToken::BaseToken(int kind, string value){
	this->kind = kind;
	this->value = value;
}
	
bool BaseToken::tokenEquals(BaseToken other){
	if(this->value.size()==0 || other.value.size()==0){
		return this->kind == other.kind;
	}
	return this->kind == other.kind && this->value.compare(other.value) == 0;
}