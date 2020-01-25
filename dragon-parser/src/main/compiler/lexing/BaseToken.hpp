#ifndef BASETOKEN
#define BASETOKEN

//standard includes
#include <vector> 
#include <stdio.h>
#include <map>

//project includes
#include "BaseToken.hpp"

using namespace std;

class BaseToken {

	/*
	It should have fields for 
	Kind,	(the kind of token, for example RPARENS)
	Value	(for example "subr" for an identifier)
	*/

	public: 
		long lineNumber = -1;
		string kind;
		string value;

	
};
#endif