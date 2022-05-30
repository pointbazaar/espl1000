#include <tables/sst/sst.h>
#include <tables/sst/sst_fill.h>
#include <tables/sst/sst_print.h>
#include <tables/stst/stst.h>
#include <tables/stst/stst_print.h>
#include "tables/symtable/symtable.h"

#include "compiler/main/util/ctx.h"
#include "flags.h"

#include "fill_tables.h"

void fill_tables(struct AST* ast, struct Ctx* ctx){

    sst_clear(ctx->tables->sst);

    for(int i = 0; i < ast->count_namespaces; i++){

        struct Namespace* ns = ast->namespaces[i];

        sst_fill(ctx->tables, ctx->tables->sst, ns);
        stst_fill(ctx->tables->stst, ns);

    }

    if(ctx->flags->debug){
        sst_print(ctx->tables->sst);
        stst_print(ctx->tables->stst);
    }
}