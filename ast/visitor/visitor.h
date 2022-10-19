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

#include "../ast_declare.h"

#define ARG void* arg

#define VISITOR void(*visitor)(void*, enum NODE_TYPE, ARG)

enum NODE_TYPE {
	NODE_AST,
	NODE_NAMESPACE,
	
	NODE_STRUCTDECL,
	NODE_STRUCTMEMBER,
	
	NODE_METHOD,
	NODE_METHOD_DECL,
	NODE_DECLARG,
	
	NODE_LAMBDA,
	
	NODE_STMTBLOCK,
	NODE_STMT,
	
	NODE_ASSIGNSTMT,
    NODE_MASSIGNSTMT,
	NODE_IFSTMT,
	NODE_WHILESTMT,
	NODE_FORSTMT,
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
    NODE_MDIRECT,
	
	NODE_INTCONST,
	NODE_BOOLCONST,
	NODE_CHARCONST,
	NODE_BINCONST,
	NODE_HEXCONST,
	NODE_CONSTVALUE,
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

void visit_ast         (struct AST* ast, 		VISITOR, ARG);

void visit_namespace   (struct Namespace* n, 	VISITOR, ARG);

void visit_method      (struct Method* m, 		VISITOR, ARG);
void visit_struct_decl  (struct StructDecl* s, 	VISITOR, ARG);

void visit_stmt_block   (struct StmtBlock* s, 	VISITOR, ARG);


//c_types_util
void visit_type        		(struct Type* t, VISITOR, ARG);
void visit_array_type   	(struct ArrayType* a, VISITOR, ARG);
void visit_subr_type    	(struct SubrType* s, VISITOR, ARG);
void visit_basic_type      	(struct BasicType* b, VISITOR, ARG);
void visit_simple_type  	(struct SimpleType* s, VISITOR, ARG);
void visit_primitive_type	(struct PrimitiveType* p, VISITOR, ARG);
void visit_struct_type		(struct StructType* s, VISITOR, ARG);
void visit_type_param   	(struct TypeParam* t, VISITOR, ARG);

#endif
