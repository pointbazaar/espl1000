#ifndef BASETOKEN
#define BASETOKEN

//standard includes
#include <string>

//project includes

using namespace std;

class BaseToken {

	/*
	It should have fields for 
	Kind,	(the kind of token, for example RPARENS)
	Value	(for example "subr" for an identifier)
	*/

	public: 
		BaseToken(string kind);
		BaseToken(string kind, float value);
		BaseToken(string kind, char value);
		BaseToken(int kind, string value);
		BaseToken(char idk);
		BaseToken(int idk);
		BaseToken(int idk, bool value);
		BaseToken(bool value);
		BaseToken();
		long lineNumber;
		string kind;
		string value;

		string getContents(){

			return this->value;
		}
	
		bool tokenEquals(BaseToken other);
};
#endif