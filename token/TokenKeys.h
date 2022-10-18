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
#define IF 	50
#define WHILE 	51

//currently unused
//#define NAMESPACE 52

#define RETURN 	53
#define STRUCT 	54
#define COMMA 	55
#define FN		57
#define ELSE 	58

#define BREAK 	59
#define CONTINUE 60

#define FOR 	61
#define IN 		62

#define SWITCH	63
#define CASE	64

#define INCLUDE_DECL 70

//operator groups
#define OPKEY_ARITHMETIC 	80
#define OPKEY_RELATIONAL	81
#define OPKEY_LOGICAL		82
#define OPKEY_BITWISE		83

//annotations
#define _ANNOT_START_		90
#define ANNOT_HALTS			91
#define ANNOT_PRIVATE		92
#define ANNOT_PUBLIC		93
#define ANNOT_DEPRECATED	94
#define _ANNOT_END_			95

#endif
