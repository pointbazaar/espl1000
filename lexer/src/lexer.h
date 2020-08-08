
//standard headers
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//user headers
#include "tokens.h"

#ifndef LEXER
#define LEXER


void free_dfa(short** dfa,int n_states);
	
void init_if_else(short** dfa);
void init_while(short** dfa);
void init_identifier(short** dfa);
void init_typeidentifier(short** dfa);

void init_single_line_comment(short** dfa);
void init_multi_line_comment(short** dfa);

void init_eq(short** dfa);
void init_numbers(short** dfa);
void init_operator(short** dfa);
void init_return(short** dfa);
void init_loop(short** dfa);
void init_charconst(short** dfa);
void init_boolconst(short** dfa);
void init_struct(short** dfa);
void init_stringconst(short** dfa);
void init_typeparam(short** dfa);
void init_arrow(short** dfa);
void init_fn(short** dfa);

//to set transitions for a state to another state in bulk
//(for some classes of chars)
void set_transitions_lowercase(short** dfa, int state, short state_result);
void set_transitions_uppercase(short** dfa, int state, short state_result);
void set_transitions_letters(short** dfa, int state, int state_result);

void set_transitions_digits(short** dfa, int state, int state_result);
void set_transitions_braces(short** dfa, int state, int state_result);
void set_transitions_operators(short** dfa, int state, int state_result);
void set_transitions_breaking(short** dfa, int state, int state_result);
void set_transitions_printable(short** dfa, int state, int state_result);
/*

Token kinds are there to avoid storing so much text about the tokens
and which kind it is in the .tokens file.

instead, the type of the token is encoded as a number

this might be harder to debug but it saves space in the file.
*/

//the empty token gets removed at the end of lexing
// it is simply for providing a starting point 
//to the linked list, to simplify a loop
static const int EMPTY = 1;

#define BOOLCONST 2

#define IDENTIFIER 4
#define TYPEIDENTIFIER 5

#define TYPEPARAM 6
#define FLOATING 7
#define INTEGER 8
#define CHARCONST 9
#define STRINGCONST 10

//this token provides the line number 
//for all tokens until the next
//LINE_NO token. could be off by several lines,
//but it's a first step.
static const int LINE_NO = 11;

#define ARROW 12

#define ANYTYPE 13
#define OPERATOR 14	//'+','-','/','*' ,'&&', '||','&','|'
#define STRUCTMEMBERACCESS 15

//keywords have their own seperate ID's
//to make the parser even faster.
//the parser should not have to look at the 
//keyword content

//#define IF 16
//can get rid of the macros :)
static const int IF=16;

#define WHILE 17
#define RETURN 21

//braces,parentheses,brackets
#define LPARENS 22
#define RPARENS 23
#define LBRACKET 24
#define RBRACKET 25
#define LCURLY 26
#define RCURLY 27

#define GREATER 28
#define LESSER 29
#define WAVE 30
#define SEMICOLON 31
#define EQ 32

#define STRUCT 34

#define COMMA 35 //','

static const int LOOP 	= 36;
static const int FN 	= 37;
static const int ELSE 	= 38;

/*

2       BoolConstantToken
3       KeywordToken
4       IdentifierToken
5       TypeIdentifierToken

6       TypeParameterIdentifierToken
7       FloatNonNegativeConstantToken
8       IntegerNonNegativeConstantToken
9       CharConstantToken
10      StringConstantToken
11      SymbolToken
12      ArrowToken

13      AnyTypeToken
14      OperatorToken
15      StructMemberAccessToken
*/


#endif
