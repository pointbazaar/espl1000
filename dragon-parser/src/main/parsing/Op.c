#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Op.h"
#include "../commandline/TokenList.h"
#include "../commandline/Token.h"
#include "../commandline/TokenKeys.h"
#include "../../../../util/util.h"

struct Op* makeOp(struct TokenList* tokens, bool debug){

	if(debug){
		printf("Op(...) from ");
		list_print(tokens);
	}

	struct Op* res = smalloc(sizeof(struct Op));

	struct TokenList* copy = list_copy(tokens);

	struct Token* tkn = list_head(copy);
	if(tkn == NULL){
		free(res);
		return NULL;
	}

	if(tkn->kind == OPKEY){

		struct Token* snd = list_get(copy,1);
		if(snd == NULL){
			free(res);
			return NULL;
		}

		if(snd->kind == OPKEY){
			struct Token* opl = tkn;
			struct Token* opr = snd;

			if(
				(
					( strcmp(opl->value,"<")==0) 
					|| strcmp(opl->value,">")==0 
				) && strcmp(opr->value,"=")==0

			){
				res->op = smalloc(sizeof(char)*3);
				res->op[2]='\0';
				strcat(res->op,opl->value);
				strcat(res->op,opr->value);
				
			}else if( strcmp(opl->value,"=") == 0 && strcmp(opr->value,"=") == 0){
				res->op = "==";
			}else{
				// "could not make operator";
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}
			if(list_size(copy) < 2){
				free(res);
				freeTokenListShallow(copy);
				return NULL;
			}
			list_consume(copy, 2);
		}else{
			res->op = tkn->value;
			
			list_consume(copy, 1);
		}
	}else{
		//"could not recognize operator, got : " + tkn->value;
		free(res);
		freeTokenListShallow(copy);
		return NULL;
	}
	
	if(debug){
		printf("sucess parsing Op\n");
	}

	list_set(tokens,copy);
	freeTokenListShallow(copy);

	return res;
}

void freeOp(struct Op* op){
	//TODO: inline op->op
	free(op);
}
