#ifndef NAMESPACE
#define NAMESPACE

#include <vector>
#include <string>

#include "Method.hpp"
#include "../commandline/TokenList.hpp"

class Namespace {

public:
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file
	Namespace(
		TokenList* tokens,
		std::string name,
		bool debug
	);


	std::string srcPath;
	std::string name;
	//structs must be declared before the subroutines
	
	//TODO: add them back later when the rewrite is complete
	//vector<StructDeclNode> structs;
	std::vector<Method> methods;
};

#endif