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

#define VISITOR void(*visitor)(void*, enum NODE_TYPE, void* arg)

enum NODE_TYPE {
	NODE_AST,
	NODE_NAMESPACE,
	
	NODE_STRUCTDECL,
	NODE_STRUCTMEMBER,
	NODE_METHOD,
	NODE_LAMBDA,
	
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
	NODE_RETSTMT,
	NODE_CONTINUESTMT,
	
	NODE_CALL,
	NODE_EXPR,
	NODE_OP,
	NODE_UNOPTERM,
	NODE_TERM,
	
	NODE_INTCONST,
	NODE_FLOATCONST,
	NODE_BOOLCONST,
	NODE_CHARCONST,
	NODE_BINCONST,
	NODE_HEXCONST,
	NODE_STRINGCONST,
	
	NODE_VARIABLE,
	NODE_SIMPLEVAR,
	
	NODE_TYPE,
	NODE_SUBRTYPE,
	NODE_SIMPLETYPE,
	NODE_STRUCTTYPE,
	NODE_PRIMITIVETYPE,
	NODE_ARRAYTYPE,
	NODE_BASICTYPE,
	NODE_TYPEPARAM,
};

void visitAST         (struct AST* ast,      VISITOR, void* arg);

void visitNamespace   (struct Namespace* n,  VISITOR, void* arg);

void visitMethod      (struct Method* m,     VISITOR, void* arg);
void visitStructDecl  (struct StructDecl* s, VISITOR, void* arg);

void visitStmtBlock   (struct StmtBlock* s,  VISITOR, void* arg);

#endif
