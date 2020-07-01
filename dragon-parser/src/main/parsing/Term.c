#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../commandline/TokenList.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"
#include "Term.h"
#include "CharConst.h"
#include "IntConst.h"
#include "BoolConst.h"
#include "FloatConst.h"
#include "Expr.h"
#include "Variable.h"
#include "statements/MethodCall.h"

struct Term* makeTerm_other(struct Expr* expr){
	struct Term* res = malloc(sizeof(struct Term));

	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;

	res->m5 = expr;
	return res;
}

struct Term* makeTerm(struct TokenList* tokens, bool debug) {

	if(debug){
		printf("Term(...) from %s\n", list_code(tokens));
	}

	struct Term* res = malloc(sizeof(struct Term));
	
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;

	struct TokenList* copy = list_copy(tokens);

	bool fail = false;
	struct TokenList* copy2 = list_copy(copy);

	if(!list_expect(copy2,LPARENS)){fail=true;}
	if(!fail){
		res->m5 = makeExpr(copy2,debug);
		if(res->m5 == NULL){fail=true;}
		if(!fail){
			if(!list_expect(copy2, RPARENS)){fail=true;}

			list_set(copy,copy2);
		}
	}

	if(fail) {
		fail=false;
		res->m2 = makeIntConst(copy,debug);
		if(res->m2 == NULL){fail = true;}

		if(fail) {
			fail = false;
		
			//a string constant is syntatic sugar.
			//in the parsing stage it is converted to an array of char constants
			//inline the stringConstant and its syntatic sugar
			struct Token* token = list_get(tokens, 0);
			
			//"Error: could not read stringConstant syntatic sugar";
			fail = true;
				
			if(fail){
				fail=false;
				res->m7 = makeFloatConst(copy,debug);
				if(res->m7 == NULL){fail = true;}

				if(fail){
					fail=false;
					res->m4 = makeMethodCall(copy,debug);
					if(res->m4 == NULL){fail = true;}

					if(fail) {
						fail=false;
						res->m1 = makeBoolConst(copy,debug);
						if(res->m1 == NULL){fail = true;}

						if(fail) {
							fail = false;
							res->m6 = makeVariable(copy,debug);
							if(res->m6 == NULL){fail=true;}

							if(fail) {
								res->m3 = makeCharConst(copy,debug);
								if(res->m3 == NULL){return NULL;}
							}
						}
					}
				}
			}
		}
	}

	list_set(tokens, copy);

	return res;
}

