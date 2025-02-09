#ifndef DEREFLL_H
#define DEREFLL_H

//struct DerefLL
//
//A representation of a chain of dereferencing and indexing access as a Linked List

//e.g.

// x1[e1][e2].x2[...]....

// &x1, +index(e1), deref, +index(e2), deref, +offset(.x2), deref

// The representation of our AST is a flatter one, a more tree-like structure.

// For TAC Compilation however, a Linked List may be prefererable,
// as it's very easy to obtain the address of the chain by
// removing the last deref. There needs no distinction if
// the last operation is indexing or member access, and
// we do not need to switch between iterating over 2 distinct continuations of the
// deref chain, (member access pointer and simplevar indices list)

// The order of what is happening also becomes clearer with this Linked List.

enum DEREFLL_ACTION {

	DEREFLL_INIT,
	DEREFLL_INDEX,
	DEREFLL_MEMBER,
	DEREFLL_DEREF,
};

struct DerefLL {
	//this struct should be immutable

	//at most 1 of these is valid for an instance
	struct SimpleVar* initial; //may be NULL
	struct Expr* index_expr; //may be NULL
	char* member_name; //may be NULL

	enum DEREFLL_ACTION action;
	struct DerefLL* next; //may be NULL

	//type is populated by special function (not ctor)
	struct Type* type;
};

struct DerefLL* derefll_ctor_variable(struct Variable* v, struct Ctx* ctx);
struct DerefLL* derefll_ctor_simplevar(struct SimpleVar* sv, struct Ctx* ctx);
void derefll_dtor(struct DerefLL* dll);

void derefll_append(struct DerefLL* d1, struct DerefLL* d2);

struct DerefLL* derefll_deref();
struct DerefLL* derefll_structmember(char* name);
struct DerefLL* derefll_expr(struct Expr* expr);

void derefll_annotate_types(struct DerefLL* dll, struct Ctx* ctx, struct Type* prev_type);

#endif
