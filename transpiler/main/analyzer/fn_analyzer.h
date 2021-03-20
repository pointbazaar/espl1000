#ifndef FN_ANALYZER_H
#define FN_ANALYZER_H

struct ST;
struct AST;

/* the function analyzer is responsible for finding
 * both callees and callers for all functions in a given
 * program.
 */
 
/* the output of the analysis is stored in the SST,
 * in the SSTLine for a given function.
 */
 
void analyze_functions(struct ST* st, struct AST* ast);

#endif
