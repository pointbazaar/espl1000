#ifndef TOKEN
#define TOKEN

#include <string>

class Token {

	/*
	 -- FIELDS --
	 - Kind 	(the kind of token, for example RPARENS)
	 - Value	(for example "subr" for an identifier)
	*/

	public: 
		Token(int kind);
		Token(int kind, std::string value);

		long lineNumber=-1;
		int kind=-1;
		std::string value="";
	
		bool tokenEquals(Token other);
};
#endif