#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "token_reader.h"
#include "../TokenKeys.h"
#include "../list/TokenList.h"
#include "../token/token.h"

static struct Token* recognizeTokenInner(int tkn_id, char* tkn, char* part2);

static struct Token* recognizeToken(char* tkn, bool* isLineNo, uint32_t* line_num);

struct TokenList* read_tokens_from_tokens_file(FILE* file, char* tokensFile) {
	
	struct TokenList* tks = makeTokenList();
	strcpy(tks->rel_path, tokensFile);
	
	size_t size = 50;
	char* line = malloc(size);
	
	uint32_t line_num = 1;
    
	while (getline(&line, &size, file)){
		
		line[strlen(line)-1] = '\0';
		
		bool isLineNo = false;
		struct Token* tkn = recognizeToken(line, &isLineNo, &line_num);
    	if(isLineNo){
			if(tkn != NULL){ 
				freeToken(tkn);
			}
			continue; 
		}
    	
    	if(tkn != NULL){
			list_add(tks, tkn);
    	}else{
    		break;
    	}
    }
	
	free(line);

	return tks;
}

static struct Token* recognizeToken(char* tkn, bool* isLineNo, uint32_t* line_num) {

	char part1[10];
	char part2[200];
	
	char* space_ptr = strchr(tkn, ' ');
	
	if(space_ptr == NULL){ return NULL; }
	
	const int space_index = space_ptr - tkn;

	tkn[space_index] = '\0';
	
	strcpy(part1, tkn);
	strcpy(part2, tkn + space_index + 1);

	const int tkn_id = atoi(part1);

	if (tkn_id == LINE_NO) {
		
		*line_num = atoi(part2);
		*isLineNo = true;
		
		return NULL;
	}
	
	struct Token* r = recognizeTokenInner(tkn_id, tkn, part2);

	if(r != NULL){
		r->line_num = *line_num;
	}

	return r;
}

static struct Token* recognizeTokenInner(int tkn_id, char* tkn, char* part2){
	
	struct Token* r = NULL;
	
	switch (tkn_id) {
		case STRINGCONST : 
			r = makeTokenStringConst(tkn+3);
			break;
		case CCONST : 
			r = makeToken2(CCONST, tkn+3);
			break;
		case ANYTYPE: 
		//CONSTANTS
		case BCONST_TRUE:
		case BCONST_FALSE:
		case FLOATING:
		case INTEGER:
		case HEXCONST:
		case BINCONST:
		//BRACKETS, BRACES, PARENTHESES
		case LBRACKET : 
		case RBRACKET : 
		case LPARENS : 
		case RPARENS : 
		case LCURLY: 
		case RCURLY:
		//IDENTIFIERS
		case ID : 
		case TYPEID:
		case TYPEID_PRIMITIVE_INT:
		case TYPEID_PRIMITIVE_BOOL:
		case TYPEID_PRIMITIVE_CHAR:
		case TYPEID_PRIMITIVE_FLOAT:
		
		//SECTION: OPERATORNS
		case OPKEY_ARITHMETIC:
		case OPKEY_RELATIONAL:
		case OPKEY_LOGICAL:
		case OPKEY_BITWISE:
		
		case ASSIGNOP: 
		//SECTION: OTHER
		case TPARAM: 
		case SEMICOLON: 
		case COMMA: 
		case ARROW: 
		case STRUCTMEMBERACCESS: 
		
		//SECTION: ANNOTATIONS
		case ANNOT_HALTS:
		case ANNOT_PRIVATE:
		case ANNOT_PUBLIC:
		case ANNOT_DEPRECATED:

		//SECTION: KEYWORDS
		case RETURN: 
		case FN: 
		case STRUCT: 
		case IF: 
		case ELSE: 
		case WHILE: 
		case LOOP: 
		case BREAK:
		case CONTINUE: 
		case FOR:
		case IN:
		case SWITCH:
		case CASE:
		case THROW: 
		case THROWS:
		case TRY:
		case CATCH:
		case EXTERNC:
		case INCLUDE_DECL:
		case RANGEOP:
		case WAVE : 
			r = makeToken2(tkn_id, part2);
			break;
		default : 
			printf("unreconized token id : %d\n", tkn_id);
			exit(1);
			return NULL;
	};
	
	return r;
}
