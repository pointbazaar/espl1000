#ifndef NAMESPACENODE
#define NAMESPACENODE

#include <vector>
#include <string>

#include "MethodNode.hpp"
#include "../commandline/TokenList.hpp"

using namespace std;

class NamespaceNode {

public:
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file
	NamespaceNode(
		TokenList* tokens,
		string name,
		bool debug
	);


	string srcPath;
	string name;
	//structs must be declared before the subroutines
	
	//TODO: add them back later when the rewrite is complete
	//vector<StructDeclNode> structs;
	vector<MethodNode> methods;
};

#endif