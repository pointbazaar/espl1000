#ifndef TOKENKEYS
#define TOKENKEYS

//Special
#define EMPTY 	1
#define LINE_NO 2

//Identifier
#define ID 				4	
#define TYPEIDENTIFIER 	5

//Type Parameter Token
#define TPARAM 		6

//Constants
#define FLOATING 	7
#define INTEGER 	8
#define CCONST 		9
#define STRINGCONST 10
#define BCONST 		11

//()[]{}
#define LPARENS 	12
#define RPARENS 	13
#define LBRACKET 	14
#define RBRACKET 	15
#define LCURLY 		16
#define RCURLY 		17

//Other
#define ARROW 		18
#define ANYTYPE 	19
#define OPKEY 		20
#define WAVE 		21
#define SEMICOLON 	22
//#define EQ 			23
#define RANGEOP 	24
#define STRUCTMEMBERACCESS 25
#define ASSIGNOP	26 //NEW

//Keywords
#define IF 		33
#define WHILE 	34
//#define NAMESPACE 35
#define RETURN 	36
#define STRUCT 	37
#define COMMA 	38
#define LOOP 	39
#define FN 		40
#define ELSE 	41
#define BREAK 	42
#define FOR 	43
#define IN 		44
#define SWITCH	45
#define CASE	46

#endif
