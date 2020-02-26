#ifndef NAMESPACE
#define NAMESPACE

struct Method;
struct StructDecl;
struct TokenList;

struct Namespace {
	//a namespace is represented by a filename.
	//the contents of a namespace are the contents of the file

	char* srcPath;
	char* name;

	//structs must be declared before the subroutines
	struct Method** methods;
	int count_methods;

	struct StructDecl** structs;
	int count_structs;
};

struct Namespace* makeNamespace(struct TokenList* tokens, char* name, bool debug);

#endif