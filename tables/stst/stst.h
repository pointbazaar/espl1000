#ifndef STRUCTSYMTABLE
#define STRUCTSYMTABLE

#include "ast/ast.h"

struct STST;

struct STSTLine {
	
	char name[DEFAULT_STR_SIZE];
	
	//where the struct was defined
	char _namespace[DEFAULT_STR_SIZE];

	struct StructDecl* decl;
	
	bool is_private;
};


struct STST* makeSTST();
void freeSTST(struct STST* stst);

struct STSTLine* makeSTSTLine(struct StructDecl* s, char* _namespace);


void             stst_clear(struct STST* stst);

void             stst_fill(struct STST* stst, struct Namespace* ns);

void             stst_add(struct STST* stst, struct STSTLine* line);

void             stst_print(struct STST* stst);

struct STSTLine* stst_get(struct STST* stst, char* name);

struct Type*     stst_get_member_type(struct STST* stst, char* struct_name, char* member_name);

#endif
