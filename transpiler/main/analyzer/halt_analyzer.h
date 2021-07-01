#ifndef HALT_ANALYZER_H
#define HALT_ANALYZER_H

/* this Analyzer Module
 * can performa a termination analysis for 
 * functions. The results, for each function are:
 * 
 *  - Terminates
 *  - Will never Terminate
 *  - Unknown
 * 
 */
 
void analyze_termination(struct ST* st);

#endif
