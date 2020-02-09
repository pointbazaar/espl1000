#ifndef AST_WHOLE_PROGRAM
#define AST_WHOLE_PROGRAM

#include <vector>
#include <string>

#include "NamespaceNode.hpp"
#include "../commandline/TokenList.hpp"

class AST_Whole_Program  {

public:
	//this contains all namespace nodes for the whole program

	std::vector<NamespaceNode> namespaceNodes;

	AST_Whole_Program(NamespaceNode myNamespace);
	AST_Whole_Program(TokenList tokens, std::string myNamespace, bool debug);
};

#endif