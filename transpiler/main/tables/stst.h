#ifndef STRUCTSYMTABLE
#define STRUCTSYMTABLE

#include "../../../ast/ast.h"

struct STSTLine {
	char name[DEFAULT_STR_SIZE];
	struct SimpleType* type;

	struct StructDecl* decl;
};

struct STST {
	//STruct Symbol Table (STST)
	
	struct STSTLine** lines;
	
	unsigned int count;
	unsigned int capacity;
};

struct STST* makeStructSymTable(struct Namespace* ns, bool debug);

struct STST* makeStructSymTable2(bool debug);

void freeStructSymTable(struct STST* stst);

void stst_add(struct STST* stst, struct StructDecl* s);

struct Type* stst_get_member_type(struct STST* stst, char* struct_name, char* member_name);

#endif
