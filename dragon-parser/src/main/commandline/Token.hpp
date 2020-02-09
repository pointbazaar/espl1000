#ifndef TOKEN
#define TOKEN

//standard includes
#include <string>

using namespace std;

class Token {

	/*
	It should have fields for 
	Kind,	(the kind of token, for example RPARENS)
	Value	(for example "subr" for an identifier)
	*/

	public: 
		Token(int kind);
		Token(int kind, string value);

		long lineNumber=-1;
		int kind=-1;
		string value="";
	
		bool tokenEquals(Token other);
};
#endif