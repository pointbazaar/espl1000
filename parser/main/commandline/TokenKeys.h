#ifndef TOKENKEYS
#define TOKENKEYS

//Special
#define EMPTY 	1
#define LINE_NO 2

//Identifier
#define ID 				4
#define TYPEID		 	5
#define TYPEID_PRIMITIVE_INT 	6
#define TYPEID_PRIMITIVE_BOOL 	7
#define TYPEID_PRIMITIVE_CHAR 	8
#define TYPEID_PRIMITIVE_FLOAT 	9

//Type Parameter Token
#define TPARAM 		10


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

//Constants
#define FLOATING 	40
#define INTEGER 	41
#define HEXCONST 	42
#define BINCONST 	43
#define CCONST 		44
#define STRINGCONST 45

#define BCONST_TRUE  46
#define BCONST_FALSE 47

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
