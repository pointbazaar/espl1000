#ifndef AST_LAMBDA_H
#define AST_LAMBDA_H

#include "../ast_declare.h"

#define LAMBDA_MAX_PARAMS 10

struct Lambda {
	
	//identifier list length is bounded
	struct Identifier* params[LAMBDA_MAX_PARAMS];
	uint8_t count_params;
	
	struct Expr* expr;
	
	//lambda number, 
	//only used when generating function
	uint32_t lambda_num;
};

#endif
