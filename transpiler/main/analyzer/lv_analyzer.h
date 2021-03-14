#ifndef LV_ANALYZER_H
#define LV_ANALYZER_H

/* this Transpiler-Analyzer Module
 * can discover the local variables of a function,
 * aswell as use the type inference Module to discover
 * their types. The results are then put into the LVST
 */


void lvst_fill(struct Method* subr, struct ST* st, bool debug);

void discoverLVStmtBlock(struct ST* st, struct StmtBlock* block);

#endif
