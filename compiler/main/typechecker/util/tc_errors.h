#ifndef TYPECHECKING_ERRORS_H
#define TYPECHECKING_ERRORS_H

struct TCCtx;
enum TC_ERR_KIND;

void error(struct TCCtx* tcctx, char* msg, enum TC_ERR_KIND err_kind);

void error_snippet(struct TCCtx* tcctx, char* snippet, enum TC_ERR_KIND err_kind);

void error_snippet_and_msg(struct TCCtx* tcctx, char* snippet, char* msg, enum TC_ERR_KIND err_kind);

#endif
