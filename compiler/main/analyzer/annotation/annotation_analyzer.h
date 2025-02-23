#pragma once

/* this Transpiler-Analyzer Module
 * can look for various Annotations
 * and provide Errors, Notes and Warnings
 * regarding those Annotations in their Context.
 */

struct AST;
struct ST;

void analyze_annotations(struct ST* st, struct AST* ast);
