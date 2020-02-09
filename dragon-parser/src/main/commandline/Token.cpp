#include <string>
#include "Token.hpp"

Token::Token(int kind){
	this->kind = kind;
}

Token::Token(int kind, std::string value){
	this->kind = kind;
	this->value = value;
}
	
bool Token::tokenEquals(Token other){
	if(this->value.size()==0 || other.value.size()==0){
		return this->kind == other.kind;
	}
	return this->kind == other.kind && this->value.compare(other.value) == 0;
}