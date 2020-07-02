#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Op.h"
#include "../commandline/TokenList.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"

struct Op* makeOp(struct TokenList* tokens, bool debug){

	if(debug){
		printf("Op(...) from %s\n", list_code(tokens, debug));
	}

	struct Op* res = malloc(sizeof(struct Op));

	struct TokenList* copy = list_copy(tokens);

	struct Token* tkn = list_get(copy, 0);
	if(tkn == NULL){return NULL;}

	if(tkn->kind == OPKEY){

		struct Token* snd = list_get(copy,1);
		if(snd == NULL){return NULL;}

		if(snd->kind == OPKEY){
			struct Token* opl = tkn;
			struct Token* opr = snd;

			if(
				(
					( strcmp(opl->value,"<")==0) 
					|| strcmp(opl->value,">")==0 
				) && strcmp(opr->value,"=")==0

			){
				res->op = malloc(sizeof(char)*3);
				res->op[2]='\0';
				strcat(res->op,opl->value);
				strcat(res->op,opr->value);
				
			}else if( strcmp(opl->value,"=") == 0 && strcmp(opr->value,"=") == 0){
				res->op = "==";
			}else{
				// "could not make operator";
				return NULL;
			}
			if(list_size(copy) < 2){
				return NULL;
			}
			list_consume(copy, 2);
		}else{
			res->op = tkn->value;
			if(list_size(copy) < 1){
				return NULL;
			}
			list_consume(copy, 1);
		}
	}else{
		//"could not recognize operator, got : " + tkn->value;
		return NULL;
	}

	list_set(tokens,copy);

	return res;
}