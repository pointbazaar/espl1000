#ifndef BASETOKEN
#define BASETOKEN

//standard includes
#include <string>

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
		BaseToken(int kind, string value);

		long lineNumber=-1;
		int kind=-1;
		string value="";
	
		bool tokenEquals(BaseToken other);
};
#endif