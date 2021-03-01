#ifndef STRUCTSYMTABLE
#define STRUCTSYMTABLE

//to include DEFAULT_STR_SIZE
#include "../../../ast/ast.h"

//TODO: finish the implementation 

struct STSTLine {
	char name[DEFAULT_STR_SIZE];
	struct SimpleType* type;

	struct StructDecl* decl;
};

struct STST {
	//STruct Symbol Table
	
	struct STSTLine** lines;
	
	unsigned int count;
	unsigned int capacity;
};

struct STST* makeStructSymTable(struct Namespace* ns, bool debug);

void freeStructSymTable(struct STST* stst);

void stst_add(struct STST* stst, struct StructDecl* s);

struct Type* stst_get_member_type(struct STST* stst, char* struct_name, char* member_name);

#endif
