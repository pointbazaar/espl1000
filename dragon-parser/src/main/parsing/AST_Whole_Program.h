#ifndef AST_WHOLE_PROGRAM
#define AST_WHOLE_PROGRAM

#include <stdbool.h>

struct Namespace;
struct TokenList;

struct AST_Whole_Program  {
	//this contains all namespace nodes for the whole program

	struct Namespace** namespaces;
	int count_namespaces;
};

struct AST_Whole_Program* makeAST_Whole_Program(struct Namespace* myNamespace);
struct AST_Whole_Program* makeAST_Whole_Program3(struct TokenList* tokens, char* myNamespace, bool debug);

#endif