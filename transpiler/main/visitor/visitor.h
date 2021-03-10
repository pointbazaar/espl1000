#ifndef VISITOR_H
#define VISITOR_H

/* this header defines a function which applies the 
 * Visitor Pattern to the AST. instead of walking through
 * the AST with one function for each node, the Visitor
 * does the visiting for you and calls a provided function
 * at each node. the handler receives the type of the AST Node
 * as a string and can then only handle the cases 
 * of interest.
 */
 
//an enum so that the handler can find out which
//node type it was called on

#define VISITOR void(*visitor)(void*, enum NODE_TYPE)

enum NODE_TYPE {
	NODE_AST,
	NODE_NAMESPACE,
	
	NODE_STRUCTDECL,
	NODE_STRUCTMEMBER,
	NODE_METHOD,
	
	NODE_STMTBLOCK,
	NODE_STMT,
	
	NODE_ASSIGNSTMT,
	NODE_IFSTMT,
	NODE_WHILESTMT,
	NODE_FORSTMT,
	NODE_LOOPSTMT,
	NODE_SWITCHSTMT,
	NODE_CASESTMT,
	NODE_BREAKSTMT,
	NODE_CONTINUESTMT,
	
	NODE_METHODCALL,
	NODE_EXPR,
	NODE_TERM,
	
	NODE_INTCONST,
	NODE_FLOATCONST,
	NODE_BINCONST,
	NODE_HEXCONST,
	NODE_STRINGCONST,
};

void visitAST         (struct AST* ast,      VISITOR);

void visitNamespace   (struct Namespace* n,  VISITOR);

void visitMethod      (struct Method* m,     VISITOR);
void visitStructDecl  (struct StructDecl* s, VISITOR);

void visitStmtBlock   (struct StmtBlock* s,  VISITOR);

#endif
