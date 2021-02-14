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
#define FLOATING 	10
#define INTEGER 	11
#define HEXCONST 	12
#define CCONST 		13
#define STRINGCONST 14

#define BCONST_TRUE  15
#define BCONST_FALSE 16

//()[]{}
#define LPARENS 	20
#define RPARENS 	21
#define LBRACKET 	22
#define RBRACKET 	23
#define LCURLY 		24
#define RCURLY 		25

//Other
#define ARROW 		30
#define ANYTYPE 	31
#define OPKEY 		32
#define WAVE 		33
#define SEMICOLON 	34
//#define EQ 		35
#define RANGEOP 	36
#define STRUCTMEMBERACCESS 37
#define ASSIGNOP	38

//Keywords
#define IF 		50
#define WHILE 	51
//#define NAMESPACE 52
#define RETURN 	53
#define STRUCT 	54
#define COMMA 	55
#define LOOP 	56
#define FN 		57
#define ELSE 	58
#define BREAK 	59
#define FOR 	60
#define IN 		61
#define SWITCH	62
#define CASE	63

#endif
