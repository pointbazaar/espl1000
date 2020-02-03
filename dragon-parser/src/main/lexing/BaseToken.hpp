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
		BaseToken();
		BaseToken(int kind);
		BaseToken(char idk);
		BaseToken(bool value);
		BaseToken(string value);
		BaseToken(int kind, int value);
		BaseToken(int kind, float value);
		BaseToken(int kind, char value);
		BaseToken(int kind, string value);
		BaseToken(int idk, bool value);

		long lineNumber;
		int kind;
		string value;

		string getContents(){

			return this->value;
		}
	
		bool tokenEquals(BaseToken other);
};
#endif