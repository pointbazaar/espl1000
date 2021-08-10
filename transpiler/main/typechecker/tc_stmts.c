#include <string.h>
#include <stdlib.h>

//AST Includes
#include "ast/ast.h"
#include "ast/util/str_ast.h"

//Typeinference Includes
#include "transpiler/main/typeinference/typeinfer.h"

//Typechecker Includes
#include "_tc.h"
#include "typechecker/util/tc_errors.h"
#include "typechecker/util/tc_utils.h"
#include "tcctx.h"

bool tc_stmt(struct Stmt* s, struct TCCtx* tcctx){

	tcctx->current_line_num = s->super.line_num;

	switch(s->kind){

        case 1: return tc_methodcall(s->ptr.m1,   tcctx);
        case 2: return tc_whilestmt(s->ptr.m2,    tcctx);
        case 3: return tc_ifstmt(s->ptr.m3,       tcctx);
        case 4: return tc_retstmt(s->ptr.m4,      tcctx);
        case 5: return tc_assignstmt(s->ptr.m5,   tcctx);
        case 6: return tc_trycatchstmt(s->ptr.m6, tcctx);
        case 7: return tc_forstmt(s->ptr.m7,      tcctx);
        case 8: return tc_switchstmt(s->ptr.m8,   tcctx);
		
		case 99:
			if(s->is_continue){ }
			if(s->is_break)   { }
            if(s->is_throw)   { return tc_throwstmt(s, tcctx); }
	}
    return true;
}

bool tc_throwstmt(struct Stmt* s, struct TCCtx* tcctx){
	
	struct Method* m = tcctx->current_fn;
				
	if(m->decl->throws) { return true; }
	
	//are we inside try-catch stmt?
	if(tcctx->depth_inside_try_stmt > 0){ return true; }
	
	char* snippet = str_stmt(s);
	
	error_snippet(tcctx, snippet, TC_ERR_THROW_WRONG_USAGE);
	
	free(snippet);
	
    return false;
}
