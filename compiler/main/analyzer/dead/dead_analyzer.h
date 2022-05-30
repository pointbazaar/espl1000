#ifndef DEAD_ANALYZER_H
#define DEAD_ANALYZER_H

/* This is the dead code analyzer
 * it looks for functions not reachable from
 * main. If there is no main, all functions
 * are assumed to be live.
 */
 
struct ST;
struct AST;

void analyze_dead_code(struct ST* st, struct AST* ast);

#endif
