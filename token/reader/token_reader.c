#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "token_reader.h"
#include "../TokenKeys.h"
#include "../list/TokenList.h"
#include "../token/token.h"

// @returns NULL on error
static struct Token* recognizeTokenInner(int tkn_id, char* tkn, char* part2);

static struct Token* recognizeToken(char* tkn, bool* isLineNo, uint32_t* line_num);

struct TokenList* read_tokens_from_tokens_file(FILE* file, char* tokensFile) {

	struct TokenList* tks = makeTokenList2(tokensFile);

	size_t size = 50;
	char* line = malloc(size);

	if (!line) {
		return NULL;
	}

	uint32_t line_num = 1;

	while (getline(&line, &size, file) > 0) {

		line[strlen(line) - 1] = '\0';

		bool isLineNo = false;
		struct Token* tkn = recognizeToken(line, &isLineNo, &line_num);
		if (isLineNo) {
			if (tkn != NULL) {
				freeToken(tkn);
			}
			continue;
		}

		if (tkn != NULL) {
			list_add(tks, tkn);
		} else {
			break;
		}
	}

	free(line);

	return tks;
}

static struct Token* recognizeTokenInnerNoStr(int tkn_id) {

	struct Token* r = NULL;

	switch (tkn_id) {
		case ANYTYPE:
		//CONSTANTS
		case BCONST_TRUE:
		case BCONST_FALSE:
		//BRACKETS, BRACES, PARENTHESES
		case LBRACKET:
		case RBRACKET:
		case LPARENS:
		case RPARENS:
		case LCURLY:
		case RCURLY:

		case TYPEID_PRIMITIVE_INT:
		case TYPEID_PRIMITIVE_UINT:
		case TYPEID_PRIMITIVE_INT8:
		case TYPEID_PRIMITIVE_UINT8:
		case TYPEID_PRIMITIVE_INT16:
		case TYPEID_PRIMITIVE_UINT16:
		case TYPEID_PRIMITIVE_INT32:
		case TYPEID_PRIMITIVE_UINT32:
		case TYPEID_PRIMITIVE_INT64:
		case TYPEID_PRIMITIVE_UINT64:
		case TYPEID_PRIMITIVE_BOOL:
		case TYPEID_PRIMITIVE_CHAR:

		//SECTION: OPERATORNS
		case OPKEY_ARITHMETIC_PLUS:
		case OPKEY_ARITHMETIC_MINUS:
		case OPKEY_ARITHMETIC_MUL:
		case OPKEY_ARITHMETIC_DIV:
		case OPKEY_RELATIONAL_EQ:
		case OPKEY_RELATIONAL_NEQ:
		case OPKEY_RELATIONAL_LT:
		case OPKEY_RELATIONAL_GT:
		case OPKEY_RELATIONAL_LE:
		case OPKEY_RELATIONAL_GE:
		case OPKEY_LOGICAL_AND:
		case OPKEY_LOGICAL_OR:
		case OPKEY_LOGICAL_NOT:
		case OPKEY_BITWISE_OR:
		case OPKEY_BITWISE_AND:
		case OPKEY_BITWISE_XOR:
		case OPKEY_BITWISE_NOT:
		case OPKEY_BITWISE_SHIFT_LEFT:
		case OPKEY_BITWISE_SHIFT_RIGHT:

		case ASSIGNOP_PLUS:
		case ASSIGNOP_MINUS:
		case ASSIGNOP_SHIFT_LEFT:
		case ASSIGNOP_SHIFT_RIGHT:
		case ASSIGNOP_BITWISE_AND:
		case ASSIGNOP_BITWISE_OR:
		case ASSIGNOP_SIMPLE:
		//SECTION: OTHER
		case SEMICOLON:
		case COMMA:
		case STRUCTMEMBERACCESS:

		//SECTION: ANNOTATIONS
		case ANNOT_HALTS:
		case ANNOT_PRIVATE:
		case ANNOT_PUBLIC:
		case ANNOT_DEPRECATED:
		case ANNOT_EXTERN:
		case ANNOT_SYSCALL:

		//SECTION: KEYWORDS
		case RETURN:
		case KEYWORD_LOCAL:
		case FN:
		case STRUCT:
		case IF:
		case ELSE:
		case WHILE:
		case KEYWORD_BREAK:
		case KEYWORD_CONTINUE:
		case FOR:
		case KEYWORD_IN:
		case RANGEOP:
		case WAVE:
		case ARROW_NO_SIDE_EFFECT:
		case ARROW_SIDE_EFFECT:
			r = makeToken(tkn_id);
			break;
		default:
			fprintf(stderr, "unrecognized token id : %d\n", tkn_id);
			return NULL;
	};

	return r;
}

static struct Token* recognizeToken(char* tkn, bool* isLineNo, uint32_t* line_num) {

	char part1[10];
	char part2[200];

	char* space_ptr = strchr(tkn, ' ');

	if (space_ptr == NULL) {
		const int tkn_id = atoi(tkn);
		struct Token* r = recognizeTokenInnerNoStr(tkn_id);

		if (r != NULL) {
			r->line_num = *line_num;
		}
		return r;
	}

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

	if (r != NULL) {
		r->line_num = *line_num;
	}

	return r;
}

static struct Token* recognizeTokenInner(int tkn_id, char* tkn, char* part2) {

	struct Token* r = NULL;

	switch (tkn_id) {
		case STRINGCONST:
			r = makeTokenStringConst(tkn + 3);
			break;
		case CCONST:
			r = makeToken2(CCONST, tkn + 3);
			break;
		case ANYTYPE:
		//CONSTANTS
		case INTEGER:
		case HEXCONST:
		case BINCONST:
		//IDENTIFIERS
		case ID:
		case TYPEID:

		//SECTION: OTHER
		case TPARAM:

		case INCLUDE_DECL:
			r = makeToken2(tkn_id, part2);
			break;
		default:
			fprintf(stderr, "unreconized token id : %d\n", tkn_id);
			return NULL;
	};

	return r;
}
