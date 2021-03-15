#ifndef FN_ANALYZER_H
#define FN_ANALYZER_H

#include "ast/ast.h"

#include "tables/sst/sst.h"
#include "tables/stst/stst.h"
#include "tables/lvst/lvst.h"
#include "tables/symtable/symtable.h"

/* the function analyzer is responsible for finding
 * both callees and callers for all functions in a given
 * program.
 */
 
/* the output of the analysis is stored in the SST,
 * in the SSTLine for a given function.
 */
 
void analyze_functions(struct ST* st, struct AST* ast);

#endif
