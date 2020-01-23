//standard includes
include <vector> 
include <stdio>
include <map>

//project includes
include "BaseToken.hpp"

public class BaseToken {

	/*
	It should have fields for 
	Kind,	(the kind of token, for example RPARENS)
	Value	(for example "subr" for an identifier)
	*/

	public: 
		long lineNumber = -1;
		string kind;
		string value;

	
}
