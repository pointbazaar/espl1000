#ifndef TOKENKEYS
#define TOKENKEYS

//Special
#define EMPTY 1
#define LINE_NO 2

//Type Parameter Token
#define TPARAM 3

//Identifier
#define ID 4

#define TYPEID 5
#define TYPEID_PRIMITIVE_BOOL 7
#define TYPEID_PRIMITIVE_CHAR 8

#define TYPEID_PRIMITIVE_INT 9
#define TYPEID_PRIMITIVE_UINT 10
#define TYPEID_PRIMITIVE_INT8 11
#define TYPEID_PRIMITIVE_UINT8 12
#define TYPEID_PRIMITIVE_INT16 13
#define TYPEID_PRIMITIVE_UINT16 14
#define TYPEID_PRIMITIVE_INT32 15
#define TYPEID_PRIMITIVE_UINT32 16
#define TYPEID_PRIMITIVE_INT64 17
#define TYPEID_PRIMITIVE_UINT64 18

//()[]{}
#define LPARENS 19
#define RPARENS 20
#define LBRACKET 21
#define RBRACKET 22
#define LCURLY 23
#define RCURLY 24

// ->
#define ARROW_NO_SIDE_EFFECT 25
// ~>
#define ARROW_SIDE_EFFECT 26

// #
#define ANYTYPE 27
// ~
#define WAVE 28
// ;
#define SEMICOLON 29
// ..
#define RANGEOP 30
// .
#define STRUCTMEMBERACCESS 31

// +=
#define ASSIGNOP_PLUS 32
// -=
#define ASSIGNOP_MINUS 33
// <<=
#define ASSIGNOP_SHIFT_LEFT 34
// >>=
#define ASSIGNOP_SHIFT_RIGHT 35
// &=
#define ASSIGNOP_BITWISE_AND 36
// |=
#define ASSIGNOP_BITWISE_OR 37
// '='
#define ASSIGNOP_SIMPLE 38

//Constants
#define INTEGER 41
#define HEXCONST 42
#define BINCONST 43
#define CCONST 44
#define STRINGCONST 45

#define BCONST_TRUE 46
#define BCONST_FALSE 47

//Keywords
#define IF 50
#define WHILE 51

//currently unused
//#define NAMESPACE 52

#define RETURN 53
#define STRUCT 54
#define COMMA 55
#define FN 57
#define ELSE 58

#define BREAK 59
#define CONTINUE 60

#define FOR 61
#define IN 62

#define INCLUDE_DECL 70

//operator groups
#define OPKEY_ARITHMETIC_PLUS 71
#define OPKEY_ARITHMETIC_MINUS 72
#define OPKEY_ARITHMETIC_MUL 73
#define OPKEY_ARITHMETIC_DIV 74

// ==
#define OPKEY_RELATIONAL_EQ 75
// !=
#define OPKEY_RELATIONAL_NEQ 76
// <
#define OPKEY_RELATIONAL_LT 77
// >
#define OPKEY_RELATIONAL_GT 78
// <=
#define OPKEY_RELATIONAL_LE 79
// >=
#define OPKEY_RELATIONAL_GE 80

// &&
#define OPKEY_LOGICAL_AND 81
// ||
#define OPKEY_LOGICAL_OR 82
// !
#define OPKEY_LOGICAL_NOT 83

// |
#define OPKEY_BITWISE_OR 84
// &
#define OPKEY_BITWISE_AND 85
// ^
#define OPKEY_BITWISE_XOR 86
// ~
#define OPKEY_BITWISE_NOT 87
// <<
#define OPKEY_BITWISE_SHIFT_LEFT 88
// >>
#define OPKEY_BITWISE_SHIFT_RIGHT 89

//annotations
#define _ANNOT_START_ 90
#define ANNOT_HALTS 91
#define ANNOT_PRIVATE 92
#define ANNOT_PUBLIC 93
#define ANNOT_DEPRECATED 94
#define _ANNOT_END_ 95

#endif
