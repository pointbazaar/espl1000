
#include <string>

#include "BaseToken.hpp"


using namespace std;

//TODO: implement

BaseToken::BaseToken(){}
BaseToken::BaseToken(int kind){}
BaseToken::BaseToken(char idk){}
BaseToken::BaseToken(bool value){}
BaseToken::BaseToken(string value){}
BaseToken::BaseToken(int kind, int value){}
BaseToken::BaseToken(int kind, float value){}
BaseToken::BaseToken(int kind, char value){}
BaseToken::BaseToken(int kind, string value){}
BaseToken::BaseToken(int idk, bool value){}

string BaseToken::getContents(){

	return this->value;
}
	
bool BaseToken::tokenEquals(BaseToken other){
	return this->value.compare(other.value)==0;
}