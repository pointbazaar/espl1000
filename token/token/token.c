#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "token.h"
#include "../TokenKeys.h"

struct Token* makeToken(int kind) {
	return makeTokenLineNo(kind, 0);
}

struct Token* makeTokenLineNo(int kind, uint32_t line_num) {

	struct Token* res = malloc(sizeof(struct Token));

	if (!res) {
		return NULL;
	}

	res->line_num = line_num;
	res->kind = kind;

	res->value_ptr = NULL;

	return res;
}

struct Token* makeToken2(int kind, char* value) {
	return makeToken2LineNo(kind, value, 0);
}

struct Token* makeToken2LineNo(int kind, char* value, uint32_t line_num) {

	struct Token* res = malloc(sizeof(struct Token));

	if (!res) {
		return NULL;
	}

	res->line_num = line_num;
	res->kind = kind;

	res->value_ptr = malloc(sizeof(char) * (strlen(value) + 1));

	if (!res->value_ptr) {
		free(res);
		return NULL;
	}

	strcpy(res->value_ptr, value);

	return res;
}

struct Token* makeTokenStringConst(char* value) {

	struct Token* res = malloc(sizeof(struct Token));

	if (!res) {
		return NULL;
	}

	res->kind = STRINGCONST;

	res->value_ptr = calloc(sizeof(char) * (strlen(value) + 1), sizeof(char));

	if (!res->value_ptr) {
		free(res);
		return NULL;
	}

	// extra '"' at start and end
	const size_t len = strlen(value) - 1;
	int j = 0;
	for (size_t i = 1; i < len; i++) {
		char c = value[i];

		// unescape '\n'
		if (c == '\\' && i + 1 < len && value[i + 1] == 'n') {
			res->value_ptr[j++] = '\n';
			i++;
		} else {
			res->value_ptr[j++] = c;
		}
	}

	res->line_num = -1;

	return res;
}

bool token_equals(struct Token* a, struct Token* b) {

	const bool kindEq = a->kind == b->kind;

	if (!kindEq) { return false; }

	if (a->value_ptr != NULL && b->value_ptr != NULL) {
		if (strlen(a->value_ptr) == 0 || strlen(b->value_ptr) == 0) {
			return true;
		} else {
			return strcmp(a->value_ptr, b->value_ptr) == 0;
		}
	}
	return true;
}

void freeToken(struct Token* token) {

	if (token->value_ptr) {
		free(token->value_ptr);
	}
	free(token);
}

char* tokenstr[255] = {
    [TPARAM] = "?T0",
    [TYPEID_PRIMITIVE_BOOL] = "bool",
    [TYPEID_PRIMITIVE_CHAR] = "char",
    [TYPEID_PRIMITIVE_INT] = "int",
    [TYPEID_PRIMITIVE_UINT] = "uint",
    [TYPEID_PRIMITIVE_INT8] = "int8",
    [TYPEID_PRIMITIVE_UINT8] = "uint8",
    [TYPEID_PRIMITIVE_INT16] = "int16",
    [TYPEID_PRIMITIVE_UINT16] = "uint16",
    [TYPEID_PRIMITIVE_INT32] = "int32",
    [TYPEID_PRIMITIVE_UINT32] = "uint32",
    [TYPEID_PRIMITIVE_INT64] = "int64",
    [TYPEID_PRIMITIVE_UINT64] = "uint64",

    [LPARENS] = "(",
    [RPARENS] = ")",
    [LBRACKET] = "[",
    [RBRACKET] = "]",
    [LCURLY] = "{",
    [RCURLY] = "}",

    [ARROW_NO_SIDE_EFFECT] = "->",
    [ARROW_SIDE_EFFECT] = "~>",

    [ANYTYPE] = "#",
    [WAVE] = "~",
    [SEMICOLON] = ";",
    [RANGEOP] = "..",
    [STRUCTMEMBERACCESS] = ".",
    [ASSIGNOP_PLUS] = "+=",
    [ASSIGNOP_MINUS] = "-=",
    [ASSIGNOP_SHIFT_LEFT] = "<<",
    [ASSIGNOP_SHIFT_RIGHT] = ">>",
    [ASSIGNOP_BITWISE_AND] = "&=",
    [ASSIGNOP_BITWISE_OR] = "|=",
    [ASSIGNOP_SIMPLE] = "=",

    [BCONST_TRUE] = "true",
    [BCONST_FALSE] = "false",

    [IF] = "if",
    [WHILE] = "while",
    [RETURN] = "return",
    [KEYWORD_LOCAL] = "local",
    [STRUCT] = "struct",
    [COMMA] = ",",
    [FN] = "fn",
    [ELSE] = "else",
    [KEYWORD_BREAK] = "break",
    [KEYWORD_CONTINUE] = "continue",
    [FOR] = "for",
    [KEYWORD_IN] = "in",

    [OPKEY_ARITHMETIC_PLUS] = "+",
    [OPKEY_ARITHMETIC_MINUS] = "-",
    [OPKEY_ARITHMETIC_MUL] = "*",
    [OPKEY_ARITHMETIC_DIV] = "/",

    [OPKEY_RELATIONAL_EQ] = "==",
    [OPKEY_RELATIONAL_NEQ] = "!=",
    [OPKEY_RELATIONAL_LT] = "<",
    [OPKEY_RELATIONAL_GT] = ">",
    [OPKEY_RELATIONAL_LE] = "<=",
    [OPKEY_RELATIONAL_GE] = ">=",

    [OPKEY_LOGICAL_AND] = "&&",
    [OPKEY_LOGICAL_OR] = "||",
    [OPKEY_LOGICAL_NOT] = "!",
    [OPKEY_BITWISE_OR] = "|",
    [OPKEY_BITWISE_AND] = "&",
    [OPKEY_BITWISE_XOR] = "^",
    [OPKEY_BITWISE_NOT] = "~",
    [OPKEY_BITWISE_SHIFT_LEFT] = "<<",
    [OPKEY_BITWISE_SHIFT_RIGHT] = ">>",

    [ANNOT_HALTS] = "@halts",
    [ANNOT_PRIVATE] = "@private",
    [ANNOT_PUBLIC] = "@public",
    [ANNOT_DEPRECATED] = "@deprecated",
    [ANNOT_EXTERN] = "@extern",

};

char* token_str(struct Token* tk) {

	if (tk->value_ptr && strlen(tk->value_ptr) != 0) {
		return tk->value_ptr;
	}

	return tokenstr[tk->kind];
}
