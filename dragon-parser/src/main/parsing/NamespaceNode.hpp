#ifndef NAMESPACENODE
#define NAMESPACENODE

#include <vector>
#include <string>

#include "StructDeclNode.hpp"
#include "MethodNode.hpp"

using namespace std;

class NamespaceNode {

public:
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	string srcPath;
	string name;
	//structs must be declared before the subroutines
	vector<StructDeclNode> structs;
	vector<MethodNode> methods;
};

#endif