#ifndef AST_WHOLE_PROGRAM
#define AST_WHOLE_PROGRAM

#include <vector>
#include <string>

#include "Namespace.hpp"
#include "../commandline/TokenList.hpp"

class AST_Whole_Program  {

public:
	//this contains all namespace nodes for the whole program

	std::vector<Namespace> namespaceNodes;

	AST_Whole_Program(Namespace myNamespace);
	AST_Whole_Program(TokenList tokens, std::string myNamespace, bool debug);
};

#endif