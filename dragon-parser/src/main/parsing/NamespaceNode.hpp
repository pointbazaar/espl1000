#ifndef NAMESPACENODE
#define NAMESPACENODE

#include <vector>
#include <string>

#include "MethodNode.hpp"
#include "../commandline/TokenList.hpp"

class NamespaceNode {

public:
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file
	NamespaceNode(
		TokenList* tokens,
		std::string name,
		bool debug
	);


	std::string srcPath;
	std::string name;
	//structs must be declared before the subroutines
	
	//TODO: add them back later when the rewrite is complete
	//vector<StructDeclNode> structs;
	std::vector<MethodNode> methods;
};

#endif