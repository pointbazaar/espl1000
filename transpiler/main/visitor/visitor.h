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

void visit_ast(struct AST* ast,  void(*visitor)(void*, char*));

#endif
