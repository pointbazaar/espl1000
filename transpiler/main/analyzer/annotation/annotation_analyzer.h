#ifndef LV_ANALYZER_H
#define LV_ANALYZER_H

/* this Transpiler-Analyzer Module
 * can look for various Annotations
 * and provide Errors, Notes and Warnings
 * regarding those Annotations in their Context.
 */

void analyze_annotations(struct ST* st, struct AST* ast);

#endif
