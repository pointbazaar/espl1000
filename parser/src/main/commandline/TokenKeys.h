#ifndef TOKENKEYS
#define TOKENKEYS


#define EMPTY 1

#define BCONST 2

//identifier
#define ID 4	
#define TYPEIDENTIFIER 5

//Type Parameter Token
#define TPARAM 6
#define FLOATING 7

#define INTEGER 8
#define CCONST 9
#define STRINGCONST 10

#define LINE_NO 11

#define ARROW 12
#define ANYTYPE 13
#define OPKEY 14
#define STRUCTMEMBERACCESS 15
#define IF 16
#define WHILE 17
//#define NAMESPACE 18
#define RETURN 21
#define LPARENS 22
#define RPARENS 23
#define LBRACKET 24
#define RBRACKET 25
#define LCURLY 26
#define RCURLY 27

#define GREATER 28 //only used in the lexer, is 'OPKEY' in parser
#define LESSER 29  //only used in the lexer, is 'OPKEY' in parser
#define WAVE 30
#define SEMICOLON 31
#define EQ 32

#define STRUCT 34
#define COMMA 35
#define LOOP 36
#define FN 37
#define ELSE 38
#define BREAK 39

#endif
