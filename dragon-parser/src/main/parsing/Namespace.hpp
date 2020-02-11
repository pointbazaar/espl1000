#ifndef NAMESPACE
#define NAMESPACE

#include <vector>
#include <string>

#include "Method.hpp"
#include "StructDecl.hpp"
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
	std::vector<Method> methods;
	std::vector<StructDecl> structs;
};

#endif