#include <set>
#include "NamespaceNode.hpp"

#ifndef AST_WHOLE_PROGRAM
#define AST_WHOLE_PROGRAM

using namespace std;

class AST_Whole_Program  {

public:
	//this contains all namespace nodes for the whole program

	set<NamespaceNode> namespaceNodes;

	AST_Whole_Program(NamespaceNode myNamespace);
	AST_Whole_Program(TokenList tokens, string namespace, bool debug);
};

#endif