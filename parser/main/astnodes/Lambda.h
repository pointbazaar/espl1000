#ifndef LAMBDA_H
#define LAMBDA_H

struct TokenList;
struct Lambda;
struct Expr;
	
struct Lambda* makeLambda(struct TokenList* tokens, bool debug);

#endif
