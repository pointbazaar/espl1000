#pragma once

//Special
#define EMPTY 0
#define LINE_NO 1

//Type Parameter Token
#define TPARAM 2

//Identifier
#define ID 3

#define TOKEN_ENUM_VALUE 4
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
//#define KEYWORD_NAMESPACE ???

#define KEYWORD_LOCAL 52
#define RETURN 53
#define STRUCT 54
#define COMMA 55
#define FN 57
#define ELSE 58

#define KEYWORD_BREAK 59
#define KEYWORD_CONTINUE 60

#define FOR 61
// 'IN' by itself seems to already be used somewhere
// so we prefix with 'KEYWORD_' here
#define KEYWORD_IN 62

#define KEYWORD_ENUM 63

//operator groups
#define OPKEY_ARITHMETIC_PLUS 71
#define OPKEY_ARITHMETIC_MINUS 72
#define OPKEY_ARITHMETIC_MUL 73
#define OPKEY_ARITHMETIC_DIV 74
#define OPKEY_ARITHMETIC_MOD 75

// ==
#define OPKEY_RELATIONAL_EQ 80
// !=
#define OPKEY_RELATIONAL_NEQ 81
// <
#define OPKEY_RELATIONAL_LT 82
// >
#define OPKEY_RELATIONAL_GT 83
// <=
#define OPKEY_RELATIONAL_LE 84
// >=
#define OPKEY_RELATIONAL_GE 85

// &&
#define OPKEY_LOGICAL_AND 86
// ||
#define OPKEY_LOGICAL_OR 87
// !
#define OPKEY_LOGICAL_NOT 88

// |
#define OPKEY_BITWISE_OR 90
// &
#define OPKEY_BITWISE_AND 91
// ^
#define OPKEY_BITWISE_XOR 92
// ~
#define OPKEY_BITWISE_NOT 93
// <<
#define OPKEY_BITWISE_SHIFT_LEFT 94
// >>
#define OPKEY_BITWISE_SHIFT_RIGHT 95

//annotations
#define _ANNOT_START_ 100
#define ANNOT_HALTS 101
#define ANNOT_PRIVATE 102
#define ANNOT_PUBLIC 103
#define ANNOT_DEPRECATED 104

// This annotation is for functions.
// The function is assumed to be defined outside of espl code.
// Do not error if the function is not defined.
// This annotation can be used to link against e.g. C code.
#define ANNOT_EXTERN 105

// This annotation is for syscalls. They are implicitely extern.
// The calling convention is slightly different for syscalls
#define ANNOT_SYSCALL 106

#define _ANNOT_END_ 107
