#ifndef LV_ANALYZER_H
#define LV_ANALYZER_H

/* this Transpiler-Analyzer Module
 * can discover the local variables of a function,
 * aswell as use the type inference Module to discover
 * their c_types_util. The results are then put into the LVST
 */

struct ST;
struct Method;

void lvst_fill(struct Method* subr, struct ST* st);

#endif
